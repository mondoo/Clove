#include "Clove/Rendering/RenderGraph/RenderGraph.hpp"

#include "Clove/Rendering/RenderGraph/RgFrameCache.hpp"
#include "Clove/Rendering/RenderGraph/RgGlobalCache.hpp"
#include "Clove/Rendering/Vertex.hpp"

#include <Clove/Graphics/GhaDescriptorSet.hpp>
#include <Clove/Graphics/GhaDescriptorSetLayout.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    RenderGraph::RenderGraph(RgFrameCache &frameCache, RgGlobalCache &globalCache)
        : frameCache{ frameCache }
        , globalCache{ globalCache } {
        frameCache.reset();
    }

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
        } else {
            CLOVE_ASSERT(false, "{0}: Trying to write to an already allocated buffer", CLOVE_FUNCTION_NAME);
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

    void RenderGraph::addGraphicsPass(GraphicsPassDescriptor passDescriptor, std::vector<RenderGraph::GraphicsSubmission> pass) {
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

        for(auto const &submission : pass) {
            if(bufferDescriptors.contains(submission.vertexBuffer.id)) {
                bufferDescriptors.at(submission.vertexBuffer.id).usageFlags |= GhaBuffer::UsageMode::VertexBuffer;
            } else {
                CLOVE_ASSERT(allocatedBuffers.contains(submission.vertexBuffer.id), "RenderGraph does not know about the vertex buffer provided!");
            }
            if(bufferDescriptors.contains(submission.indexBuffer.id)) {
                bufferDescriptors.at(submission.indexBuffer.id).usageFlags |= GhaBuffer::UsageMode::IndexBuffer;
            } else {
                CLOVE_ASSERT(allocatedBuffers.contains(submission.indexBuffer.id), "RenderGraph does not know about the index buffer provided!");
            }

            for(auto const &ubo : submission.shaderUbos) {
                if(bufferDescriptors.contains(ubo.buffer.id)) {
                    bufferDescriptors.at(ubo.buffer.id).usageFlags |= GhaBuffer::UsageMode::UniformBuffer;
                } else {
                    CLOVE_ASSERT(allocatedBuffers.contains(ubo.buffer.id), "RenderGraph does not know about UBO at slot {0}", ubo.slot);
                }
            }
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
        descriptorSetLayouts[pipelineId] = globalCache.createDescriptorSetLayout(GhaDescriptorSetLayout::Descriptor{ .bindings = std::move(descriptorBindings) });

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

        //TODO: PassDescriptors are likely to be reused - if that's the case then we can just use the same pipeline ID
        allocatedPipelines[pipelineId] = globalCache.createGraphicsPipelineObject(GhaGraphicsPipelineObject::Descriptor{
            .vertexShader         = allocatedShaders.at(passDescriptor.vertexShader),
            .pixelShader          = allocatedShaders.at(passDescriptor.pixelShader),
            .vertexInput          = Vertex::getInputBindingDescriptor(),
            .vertexAttributes     = vertexAttributes,
            .viewportDescriptor   = viewScissorArea,
            .scissorDescriptor    = viewScissorArea,
            .renderPass           = allocatedRenderPasses.at(pipelineId),
            .descriptorSetLayouts = { descriptorSetLayouts.at(pipelineId) },
            .pushConstants        = {},
        });

        passDescriptors[pipelineId] = std::move(passDescriptor);//NOTE: This will duplicate any descriptors
        passSubmissions[pipelineId] = std::move(pass);

        //Record draw calls
        operations.emplace_back([this, pipelineId](GhaGraphicsCommandBuffer &graphicsBuffer, GhaComputeCommandBuffer &computeBuffer, GhaTransferCommandBuffer &transferbuffer) {
            GraphicsPassDescriptor const &passDescriptor{ passDescriptors.at(pipelineId) };

            //TODO: Batch operations by render pass and start the pass outside of the operation
            RenderArea renderArea{
                .origin = passDescriptor.viewportPosition,
                .size   = passDescriptor.viewportSize,
            };

            std::vector<ClearValue> clearValues{};
            for(auto &target : passDescriptor.renderTargets) {
                clearValues.push_back(target.clearColour);
            }
            clearValues.push_back(passDescriptor.depthStencil.clearValue);

            graphicsBuffer.beginRenderPass(*allocatedRenderPasses.at(pipelineId), *allocatedFramebuffers.at(pipelineId), renderArea, clearValues);

            //TODO: Only do this if viewport is dynamic
            graphicsBuffer.setViewport(passDescriptor.viewportPosition, passDescriptor.viewportSize);
            graphicsBuffer.setScissor({ 0, 0 }, passDescriptor.viewportSize);

            graphicsBuffer.bindPipelineObject(*allocatedPipelines.at(pipelineId));

            for(size_t index{ 0 }; auto &submission : passSubmissions.at(pipelineId)) {
                auto const &passDescriptor{ allocatedDescriptorSets.at(pipelineId)[index] };

                for(auto const &ubo : submission.shaderUbos) {
                    //TODO: Handle different allocations within the same buffer
                    passDescriptor->map(*allocatedBuffers.at(ubo.buffer.id), 0, bufferDescriptors.at(ubo.buffer.id).size, DescriptorType::UniformBuffer, ubo.slot);
                }
                for(auto const &imageSampler : submission.shaderCombinedImageSamplers) {
                    passDescriptor->map(*allocatedImageViews.at(imageSampler.image), *allocatedSamplers.at(imageSampler.image), GhaImage::Layout::ShaderReadOnlyOptimal, imageSampler.slot);
                }

                graphicsBuffer.bindDescriptorSet(*passDescriptor, 0);//TODO: Multiple sets / only set sets for a whole pass (i.e. view)

                graphicsBuffer.bindVertexBuffer(*allocatedBuffers.at(submission.vertexBuffer.id), submission.vertexBuffer.offset);
                graphicsBuffer.bindIndexBuffer(*allocatedBuffers.at(submission.indexBuffer.id), submission.indexBuffer.offset, IndexType::Uint16);

                graphicsBuffer.drawIndexed(submission.indexCount);

                ++index;
            }

            graphicsBuffer.endRenderPass();
        });
    }

    void RenderGraph::addComputePass(ComputePassDescriptor const &passDescriptor, std::vector<RenderGraph::ComputeSubmission> pass) {
        //TODO
    }

    GraphicsSubmitInfo RenderGraph::execute() {
        //TEMP: Create all resources. Later we should only create the ones that are consumed
        
        //Allocate buffers
        for(auto &&[id, descriptor] : bufferDescriptors) {
            allocatedBuffers[id] = frameCache.allocateBuffer(descriptor);
        }

        //Allocate images
        for(auto &&[id, descriptor] : imageDescriptors) {
            allocatedImages[id] = frameCache.allocateImage(descriptor);
        }

        //Allocate frame buffers
        //TODO: Currently there'll be a unique frame buffer per pass. If a group of passes write to the same images, they should use the same FB
        for(auto &&[id, descriptor] : passDescriptors) {
            std::vector<std::shared_ptr<GhaImageView>> attachments{};
            for(auto &renderTarget : descriptor.renderTargets) {
                //TEMP: Just create the view when we need it
                if(!allocatedImageViews.contains(renderTarget.target)) {
                    allocatedImageViews[renderTarget.target] = frameCache.allocateImageView(allocatedImages.at(renderTarget.target).get(), GhaImageView::Descriptor{ .type = GhaImageView::Type::_2D });
                }
                attachments.push_back(allocatedImageViews.at(renderTarget.target));
            }
            //TEMP: Just create the view when we need it
            if(!allocatedImageViews.contains(descriptor.depthStencil.target)) {
                allocatedImageViews[descriptor.depthStencil.target] = frameCache.allocateImageView(allocatedImages.at(descriptor.depthStencil.target).get(), GhaImageView::Descriptor{ .type = GhaImageView::Type::_2D });
            }
            attachments.push_back(allocatedImageViews.at(descriptor.depthStencil.target));

            allocatedFramebuffers[id] = frameCache.allocateFramebuffer(GhaFramebuffer::Descriptor{
                .renderPass  = allocatedRenderPasses[id],
                .attachments = std::move(attachments),
                .width       = getImageSize(descriptor.renderTargets[0].target).x,//TEMP: Just using the first target as the size. This will need to be validated
                .height      = getImageSize(descriptor.renderTargets[0].target).y,
            });
        }

        //Allocate samplers
        for(auto &&[id, pass] : passSubmissions) {
            for(auto const &submission : pass){
                for(auto const &imageSampler : submission.shaderCombinedImageSamplers){
                    allocatedSamplers[id] = globalCache.createSampler(imageSampler.samplerState);
                }
            }
        }

        //Allocate descriptor sets
        //first pass - count all the different descriptor types
        std::unordered_map<DescriptorType, uint32_t> bindingCount{};
        uint32_t totalSets{ 0 };
        for(auto &&[id, submissions] : passSubmissions) {
            for(auto &submission : submissions) {
                bool const hasUbo{ !submission.shaderUbos.empty() };
                bool const hasImageSampler{ !submission.shaderCombinedImageSamplers.empty() };

                if(hasUbo) {
                    bindingCount[DescriptorType::UniformBuffer] += submission.shaderUbos.size();
                }
                if(hasImageSampler) {
                    bindingCount[DescriptorType::CombinedImageSampler] += submission.shaderCombinedImageSamplers.size();
                }

                if(hasUbo || hasImageSampler) {
                    ++totalSets;//Allocating a single set per submission
                }
            }
        }

        //Second pass - create a single pool to allocate from
        std::vector<DescriptorInfo> descriptorTypes{};
        descriptorTypes.reserve(bindingCount.size());

        for(auto &&[type, count] : bindingCount) {
            descriptorTypes.emplace_back(DescriptorInfo{
                .type  = type,
                .count = count,
            });
        }

        std::shared_ptr<GhaDescriptorPool> descriptorPool{ frameCache.allocateDescriptorPool(GhaDescriptorPool::Descriptor{
            .poolTypes = std::move(descriptorTypes),
            .flag      = GhaDescriptorPool::Flag::None,
            .maxSets   = totalSets,
        }) };
        descriptorPool->reset();//Make sure we have a fresh pool. TODO: do this inside frame cache? Or have a function to reset states

        //Third pass - create a descriptor set for each pipeline
        for(auto &&[id, pipeline] : allocatedPipelines) {
            std::vector<std::shared_ptr<GhaDescriptorSetLayout>> layouts(passSubmissions.at(id).size(), pipeline->getDescriptor().descriptorSetLayouts[0]);//TEMP: Using first index as we know pipelines always have a single descriptor for now
            allocatedDescriptorSets[id] = descriptorPool->allocateDescriptorSets(layouts);
        }

        //Build a graphics submission
        std::shared_ptr<GhaGraphicsCommandBuffer> graphicsCommandBufffer{ frameCache.getGraphicsCommandBuffer() };
        std::shared_ptr<GhaComputeCommandBuffer> computeCommandBufffer{ frameCache.getComputeCommandBuffer() };
        std::shared_ptr<GhaTransferCommandBuffer> transferCommandBufffer{ frameCache.getTransferCommandBuffer() };

        //TEMP: Just using the graphics for now
        graphicsCommandBufffer->beginRecording(CommandBufferUsage::OneTimeSubmit);
        for(auto &operation : operations) {
            operation(*graphicsCommandBufffer, *computeCommandBufffer, *transferCommandBufffer);
        }
        graphicsCommandBufffer->endRecording();

        //TODO: Perhaps the render graph should take a render target and do this internally.
        return GraphicsSubmitInfo{
            .commandBuffers = { graphicsCommandBufffer },
        };
    }

    GhaImage::Format RenderGraph::getImageFormat(RgImage image) {
        if(imageDescriptors.contains(image)) {
            return imageDescriptors.at(image).format;
        } else if(allocatedImages.contains(image)) {
            return allocatedImages.at(image)->getDescriptor().format;
        } else {
            return allocatedImageViews.at(image)->getImageFormat();
        }
    }

    vec2ui RenderGraph::getImageSize(RgImage image) {
        if(imageDescriptors.contains(image)) {
            return imageDescriptors.at(image).dimensions;
        } else if(allocatedImages.contains(image)) {
            return allocatedImages.at(image)->getDescriptor().dimensions;
        } else {
            return allocatedImageViews.at(image)->getImageDimensions();
        }
    }
}