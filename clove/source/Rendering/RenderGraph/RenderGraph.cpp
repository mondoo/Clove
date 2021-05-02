#include "Clove/Rendering/RenderGraph/RenderGraph.hpp"

#include "Clove/Rendering/RenderGraph/RgFrameCache.hpp"
#include "Clove/Rendering/RenderGraph/RgGlobalCache.hpp"
#include "Clove/Rendering/Vertex.hpp"

#include <Clove/Graphics/GhaDescriptorSetLayout.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    RenderGraph::RenderGraph(RgFrameCache &frameCache, RgGlobalCache &globalCache)
        : frameCache{ frameCache }
        , globalCache{ globalCache } {}

    RenderGraph::RenderGraph(RenderGraph const &other) = default;

    RenderGraph::RenderGraph(RenderGraph &&other) noexcept = default;

    RenderGraph &RenderGraph::operator=(RenderGraph const &other) = default;

    RenderGraph &RenderGraph::operator=(RenderGraph &&other) noexcept = default;

    RenderGraph::~RenderGraph() = default;

    RgBuffer RenderGraph::createBuffer(size_t bufferSize) {
        RgBuffer const buffer{ nextId++ };
        bufferDescriptors[buffer.id] = GhaBuffer::Descriptor{
            .size        = bufferSize,
            .usageFlags  = static_cast<GhaBuffer::UsageMode>(0),//Will be built when executing the graph
            .sharingMode = SharingMode::Exclusive,              //Assume exclusive to begin with
            .memoryType  = MemoryType::VideoMemory,             //Assume video memory until it has been written to from the host
        };

        return buffer;
    }

    RgBuffer RenderGraph::createBuffer(std::shared_ptr<GhaBuffer> buffer, size_t offset, size_t size) {
        RgBuffer const rgBuffer{ nextId++, offset, size };
        allocatedBuffers[rgBuffer.id] = std::move(buffer);

        return rgBuffer;
    }

    void RenderGraph::writeToBuffer(RgBuffer const &buffer, void const *data, size_t const offset, size_t const size) {
        BufferWrite write{
            .data   = std::vector<std::byte>(size),
            .offset = offset,
            .size   = size,
        };
        memcpy(write.data.data(), data, size);
        bufferWrites[buffer.id] = std::move(write);

        if(!allocatedBuffers.contains(buffer.id)) {
            //Update to system memory if a write has been recorded.
            bufferDescriptors[buffer.id].memoryType = MemoryType::SystemMemory;
        }

        operations.emplace_back([this, buffer](GhaGraphicsCommandBuffer &graphicsBuffer, GhaComputeCommandBuffer &computeBuffer, GhaTransferCommandBuffer &transferbuffer) {
            BufferWrite &write{ bufferWrites.at(buffer.id) };
            allocatedBuffers[buffer.id]->write(write.data.data(), buffer.offset + write.offset, write.size);
        });
    }

    RgImage RenderGraph::createImage(GhaImage::Type imageType, GhaImage::Format format, vec2ui dimensions) {
        RgImage const image{ nextId++ };
        imageDescriptors[image] = GhaImage::Descriptor{
            .type        = imageType,
            .usageFlags  = static_cast<GhaImage::UsageMode>(0),//Will be built when executing the graph
            .dimensions  = dimensions,
            .format      = format,
            .sharingMode = SharingMode::Exclusive,//Assume exclusive to beigin with
        };

        return image;
    }

    RgImage RenderGraph::createImage(std::shared_ptr<GhaImageView> ghaImageView) {
        RgImage const image{ nextId++ };
        allocatedImageViews[image] = std::move(ghaImageView);

        return image;
    }

    RgShader RenderGraph::createShader(std::filesystem::path const &file, GhaShader::Stage shaderStage) {
        RgShader const shader{ nextId++ };
        allocatedShaders[shader] = globalCache.createShader(file, shaderStage);//Allocate straight away as it's usage does not define it's properties

        return shader;
    }

    RgShader RenderGraph::createShader(std::string_view source, std::unordered_map<std::string, std::string> includeSources, std::string_view shaderName, GhaShader::Stage shaderStage) {
        RgShader const shader{ nextId++ };
        allocatedShaders[shader] = globalCache.createShader(source, std::move(includeSources), shaderName, shaderStage);//Allocate straight away as it's usage does not define it's properties

        return shader;
    }

    void RenderGraph::addGraphicsPass(GraphicsPassDescriptor const &passDescriptor, std::vector<RenderGraph::GraphicsSubmission> pass) {
        //Allocate a resource ID for this whole pass
        ResourceIdType const pipelineId{ nextId++ };//TODO: This will allocate one with a new id even if it's been cached. Needs changing

        //Update resource usage
        for(auto &renderTarget : passDescriptor.renderTargets) {
            if(imageDescriptors.contains(renderTarget.target)) {
                imageDescriptors.at(renderTarget.target).usageFlags |= GhaImage::UsageMode::ColourAttachment;
            } else {
                //Validate that the image exists
                CLOVE_ASSERT(allocatedImageViews.contains(renderTarget.target), "RenderGraph does not know about the render target provided!");
            }
        }
        if(imageDescriptors.contains(passDescriptor.depthStencil.target)) {
            imageDescriptors.at(passDescriptor.depthStencil.target).usageFlags |= GhaImage::UsageMode::DepthStencilAttachment;
        } else {
            //Validate that the image exists
            CLOVE_ASSERT(allocatedImageViews.contains(passDescriptor.depthStencil.target), "RenderGraph does not know about the depth stencil target provided!");
        }

        //Build the render pass
        std::vector<AttachmentDescriptor> colourAttachments{};
        for(auto &renderTarget : passDescriptor.renderTargets) {
            colourAttachments.emplace_back(AttachmentDescriptor{
                .format         = getImageFormat(renderTarget.target),
                .loadOperation  = renderTarget.loadOp,
                .storeOperation = renderTarget.storeOp,
                .initialLayout  = GhaImage::Layout::Undefined,//TODO: Can't be set properly until graph is executed?
                .usedLayout     = GhaImage::Layout::ColourAttachmentOptimal,
                .finalLayout    = GhaImage::Layout::Present,//TODO: Can't be set properly until graph is executed?
            });
        }

        AttachmentDescriptor depthStencilAttachment{
            .format         = getImageFormat(passDescriptor.depthStencil.target),
            .loadOperation  = passDescriptor.depthStencil.loadOp,
            .storeOperation = passDescriptor.depthStencil.storeOp,
            .initialLayout  = GhaImage::Layout::Undefined,//TODO: Can't be set properly until graph is executed?
            .usedLayout     = GhaImage::Layout::DepthStencilAttachmentOptimal,
            .finalLayout    = GhaImage::Layout::DepthStencilAttachmentOptimal,//TODO: Can't be set properly until graph is executed?
        };
        allocatedRenderPasses[pipelineId] = globalCache.createRenderPass(GhaRenderPass::Descriptor{ .colourAttachments = std::move(colourAttachments), .depthAttachment = std::move(depthStencilAttachment) });

        //Build descriptor layouts using the first pass. TODO: Get this infomation from shader reflection
        std::vector<DescriptorSetBindingInfo> descriptorBindings{};
        for(auto &ubo : pass[0].shaderUbos) {
            descriptorBindings.emplace_back(DescriptorSetBindingInfo{
                .binding   = ubo.slot,
                .type      = DescriptorType::UniformBuffer,
                .arraySize = 1,
                .stage     = GhaShader::Stage::Vertex,//TODO: provided by pass or by shader reflection
            });
        }
        for(auto &sampler : pass[0].shaderCombinedImageSamplers) {
            descriptorBindings.emplace_back(DescriptorSetBindingInfo{
                .binding   = sampler.slot,
                .type      = DescriptorType::CombinedImageSampler,
                .arraySize = 1,
                .stage     = GhaShader::Stage::Pixel,//TODO: provided by pass or by shader reflection
            });
        }
        std::shared_ptr<GhaDescriptorSetLayout> descriptorSetLayout{ globalCache.createDescriptorSetLayout(GhaDescriptorSetLayout::Descriptor{ .bindings = std::move(descriptorBindings) }) };

        //Build graphics pipeline
        //TODO: Hard coding in layout - will likely need to get it from the shader
        std::vector<VertexAttributeDescriptor> const vertexAttributes{
            VertexAttributeDescriptor{
                .format = VertexAttributeFormat::R32G32B32_SFLOAT,
                .offset = offsetof(Vertex, position),
            },
        };

        //TEMP: Using dynamic for now
        AreaDescriptor const viewScissorArea{
            .state = ElementState::Dynamic,
        };

        allocatedPipelines[pipelineId] = globalCache.createGraphicsPipelineObject(GhaGraphicsPipelineObject::Descriptor{
            .vertexShader         = allocatedShaders.at(passDescriptor.vertexShader),
            .pixelShader          = allocatedShaders.at(passDescriptor.pixelShader),
            .vertexInput          = Vertex::getInputBindingDescriptor(),
            .vertexAttributes     = vertexAttributes,
            .viewportDescriptor   = viewScissorArea,
            .scissorDescriptor    = viewScissorArea,
            .renderPass           = allocatedRenderPasses.at(pipelineId),
            .descriptorSetLayouts = { std::move(descriptorSetLayout) },
            .pushConstants        = {},
        });

        //Record draw calls
        vec2ui const renderSize{ getImageSize(pass[0].renderTargets[0].target) };//TEMP: Use the size of the first target
        operations.emplace_back([this, pipelineId, pass = pass, renderSize](GhaGraphicsCommandBuffer &graphicsBuffer, GhaComputeCommandBuffer &computeBuffer, GhaTransferCommandBuffer &transferbuffer) { 
            //TODO: Batch operations by render pass and start the pass outside of the operation
            RenderArea renderArea{
                .origin = { 0, 0 },
                .size   = renderSize
            };
            std::span<ClearValue> clearValues{};
            graphicsBuffer.beginRenderPass(*allocatedRenderPasses.at(pipelineId), *allocatedFramebuffers.at(pipelineId), renderArea, clearValues);

            //TODO: Only do this if viewport is dynamic
            graphicsBuffer.setViewport({ 0, 0 }, renderSize);
            graphicsBuffer.setScissor({ 0, 0 }, renderSize);

            graphicsBuffer.bindPipelineObject(*allocatedPipelines.at(pipelineId));

            for(auto &submission : pass) {
                graphicsBuffer.bindDescriptorSet(*allocatedDescriptorSets.at(pipelineId), 0);//TODO: Multiple sets / only set sets for a whole pass (i.e. view)

                graphicsBuffer.bindVertexBuffer(*allocatedBuffers.at(submission.vertexBuffer.id), submission.vertexBuffer.offset);
                graphicsBuffer.bindIndexBuffer(*allocatedBuffers.at(submission.indexBuffer.id), submission.indexBuffer.offset, IndexType::Uint16);

                graphicsBuffer.drawIndexed(submission.indexCount);
            }

            graphicsBuffer.endRenderPass();
        });
    }

    void RenderGraph::addComputePass(ComputePassDescriptor const &passDescriptor, std::vector<RenderGraph::ComputeSubmission> pass) {
        //TODO
    }

    //Return a submit info?
    GraphicsSubmitInfo RenderGraph::execute(GhaGraphicsQueue &graphicsQueue, GhaComputeQueue &computeQueue, GhaTransferQueue &transferQueue) {
        //TEMP: Create all resources. Later we should only create the ones that are consumed
        for(auto &&[id, descriptor] : bufferDescriptors) {
            allocatedBuffers[id] = frameCache.allocateBuffer(descriptor);
        }
        for(auto &&[id, descriptor] : imageDescriptors) {
            allocatedImages[id] = frameCache.allocateImage(descriptor);
        }

        return {};
    }

    GhaImage::Format RenderGraph::getImageFormat(RgImage image) {
        if(imageDescriptors.contains(image)) {
            return imageDescriptors.at(image).format;
        } else {
            return allocatedImages.at(image)->getDescriptor().format;
        }
    }

    vec2ui RenderGraph::getImageSize(RgImage image) {
        if(imageDescriptors.contains(image)) {
            return imageDescriptors.at(image).dimensions;
        } else {
            return allocatedImages.at(image)->getDescriptor().dimensions;
        }
    }
}