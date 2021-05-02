#include "Clove/Rendering/RenderGraph/RenderGraph.hpp"

#include "Clove/Rendering/RenderGraph/RgFrameCache.hpp"
#include "Clove/Rendering/RenderGraph/RgGlobalCache.hpp"
#include "Clove/Rendering/Vertex.hpp"

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

        GhaBuffer::Descriptor &descriptor{ bufferDescriptors[buffer.id] };
        descriptor.size        = bufferSize;
        descriptor.usageFlags  = static_cast<GhaBuffer::UsageMode>(0);//Will be built when executing the graph
        descriptor.sharingMode = SharingMode::Exclusive;              //Assume exclusive to begin with
        descriptor.memoryType  = MemoryType::VideoMemory;             //Assume video memory until it has been written to from the host

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

        operations.emplace_back([this, buffer]() {
            BufferWrite &write{ bufferWrites.at(buffer.id) };
            allocatedBuffers[buffer.id]->write(write.data.data(), buffer.offset + write.offset, write.size);
        });
    }

    RgImage RenderGraph::createImage(GhaImage::Type imageType, GhaImage::Format format, vec2ui dimensions) {
        RgImage const image{ nextId++ };

        GhaImage::Descriptor &imageDescriptor{ imageDescriptors[image] };
        imageDescriptor.type        = imageType;
        imageDescriptor.usageFlags  = static_cast<GhaImage::UsageMode>(0);//Will be built when executing the graph
        imageDescriptor.dimensions  = dimensions;
        imageDescriptor.format      = format;
        imageDescriptor.sharingMode = SharingMode::Exclusive;//Assume exclusive to beigin with

        return image;
    }

    RgImage RenderGraph::createImage(std::shared_ptr<GhaImageView> ghaImageView) {
        RgImage const image{ nextId++ };
        allocatedImages[image] = std::move(ghaImageView);

        return image;
    }

    RgShader RenderGraph::createShader(std::filesystem::path const &file, GhaShader::Stage shaderStage) {
        RgShader const shader{ nextId++ };
        //Allocate straight away as it's usage does not define it's properties
        allocatedShaders[shader] = globalCache.createShader(file, shaderStage);

        return shader;
    }

    RgShader RenderGraph::createShader(std::string_view source, std::unordered_map<std::string, std::string> includeSources, std::string_view shaderName, GhaShader::Stage shaderStage) {
        RgShader const shader{ nextId++ };
        //Allocate straight away as it's usage does not define it's properties
        allocatedShaders[shader] = globalCache.createShader(source, std::move(includeSources), shaderName, shaderStage);

        return shader;
    }

    RgGraphicsPipelineState RenderGraph::createGraphicsPipelineState(RgGraphicsPipelineState::Descriptor desciptor) {
        RgGraphicsPipelineState const pipeline{ nextId++ };
        GhaGraphicsPipelineObject::Descriptor pipelineDescriptor{};

        std::vector<AttachmentDescriptor> colourAttachments{};
        AttachmentDescriptor depthAttachment{};

        //Update usage flags of the attached resources and build the render pass attachment array
        for(auto &renderTarget : desciptor.renderTargets) {
            RgImage const target{ renderTarget.target };
            if(imageDescriptors.contains(target)) {
                auto &targetDesc{ imageDescriptors.at(target) };
                targetDesc.usageFlags |= GhaImage::UsageMode::ColourAttachment;

                colourAttachments.emplace_back(AttachmentDescriptor{
                    .format         = targetDesc.format,
                    .loadOperation  = renderTarget.loadOp,
                    .storeOperation = renderTarget.storeOp,
                    .initialLayout  = GhaImage::Layout::Undefined,
                    .usedLayout     = GhaImage::Layout::ColourAttachmentOptimal,
                    .finalLayout    = GhaImage::Layout::Present,//TODO: Assuming present. This will cause issues with shadow maps etc.
                });

            } else {
                //Validate that the image exists
                CLOVE_ASSERT(allocatedImages.contains(target), "RenderGraph does not know about the render target provided!");
            }
        }

        {
            RgImage const depthStencil{ desciptor.depthStencil.target };
            if(imageDescriptors.contains(depthStencil)) {
                auto &depthStencilDesc{ imageDescriptors.at(depthStencil) };
                depthStencilDesc.usageFlags |= GhaImage::UsageMode::DepthStencilAttachment;

                depthAttachment.format         = depthStencilDesc.format;
                depthAttachment.loadOperation  = desciptor.depthStencil.loadOp;
                depthAttachment.storeOperation = desciptor.depthStencil..storeOp;
                depthAttachment.initialLayout  = GhaImage::Layout::Undefined;
                depthAttachment.usedLayout     = GhaImage::Layout::DepthStencilAttachmentOptimal;
                depthAttachment.finalLayout    = GhaImage::Layout::DepthStencilAttachmentOptimal;
            } else {
                //Validate that the image exists
                CLOVE_ASSERT(allocatedImages.contains(depthStencil), "RenderGraph does not know about the depth stencil target provided!");
            }
        }

        //Compile the shaders for the pipeline
        pipelineDescriptor.vertexShader = allocatedShaders[desciptor.vertexShader];
        pipelineDescriptor.pixelShader  = allocatedShaders[desciptor.pixelShader];

        //Build vertex input around Clove's vertex structure
        std::vector<VertexAttributeDescriptor> vertexAttributes{ //TODO: Shader reflection to get proper input?
            VertexAttributeDescriptor{
                .format = VertexAttributeFormat::R32G32B32_SFLOAT,
                .offset = offsetof(Vertex, position),
            }
        };

        pipelineDescriptor.vertexInput = Vertex::getInputBindingDescriptor();
        pipelineDescriptor.vertexAttributes = std::move(vertexAttributes);

        //Create the render pass for the pipeline
        pipelineDescriptor.renderPass = globalCache.createRenderPass(GhaRenderPass::Descriptor{
            .colourAttachments = std::move(colourAttachments),
            .depthAttachment   = std::move(depthAttachment),
        });

        //TEMP: Just using dynamic for now
        AreaDescriptor const viewScissorArea{
            .state = ElementState::Dynamic,
        };
        pipelineDescriptor.viewportDescriptor = viewScissorArea;
        pipelineDescriptor.scissorDescriptor  = viewScissorArea;

        //Cache the descriptor to create the pipeline for later as we do not know the descriptor sets / push constants at this point
        pendingGraphicsPipelines[pipeline.id] = std::move(pipelineDescriptor);

        return pipeline;
    }

    RgComputePipelineState RenderGraph::createComputePipelineState(RgComputePipelineState::Descriptor descriptor) {
        //TODO

        return {};
    }

    void RenderGraph::addGraphicsPass(RgGraphicsPipelineState pipelineState, std::vector<RenderGraph::GraphicsSubmission> pass) {
    }

    void RenderGraph::addComputePass(RgComputePipelineState pipelineState, std::vector<RenderGraph::ComputeSubmission> pass) {
        //TODO
    }

    //Return a submit info?
    GraphicsSubmitInfo RenderGraph::execute(GhaFactory &factory, GhaGraphicsQueue &graphicsQueue, GhaComputeQueue &computeQueue, GhaTransferQueue &transferQueue) {
        //TODO

        return {};
    }
}