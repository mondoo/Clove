#include "Clove/Rendering/RenderGraph/RenderGraph.hpp"

#include "Clove/Rendering/RenderGraph/RgFrameCache.hpp"
#include "Clove/Rendering/RenderGraph/RgGlobalCache.hpp"
#include "Clove/Rendering/Vertex.hpp"

#include <Clove/Graphics/GhaComputeCommandBuffer.hpp>
#include <Clove/Graphics/GhaComputePipelineObject.hpp>
#include <Clove/Graphics/GhaComputeQueue.hpp>
#include <Clove/Graphics/GhaDescriptorSet.hpp>
#include <Clove/Graphics/GhaDescriptorSetLayout.hpp>
#include <Clove/Graphics/GhaGraphicsCommandBuffer.hpp>
#include <Clove/Graphics/GhaGraphicsPipelineObject.hpp>
#include <Clove/Graphics/GhaRenderPass.hpp>
#include <Clove/Graphics/GhaSampler.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    RenderGraph::RenderGraph(RgFrameCache &frameCache, RgGlobalCache &globalCache)
        : frameCache{ frameCache }
        , globalCache{ globalCache } {
        frameCache.reset();
    }

    RenderGraph::~RenderGraph() = default;

    RgResourceIdType RenderGraph::createBuffer(size_t const bufferSize) {
        RgResourceIdType const bufferId{ nextResourceId++ };
        buffers[bufferId] = std::make_unique<RgBuffer>(bufferId, bufferSize);

        return bufferId;
    }

    RgResourceIdType RenderGraph::createBuffer(std::shared_ptr<GhaBuffer> buffer, size_t const offset, size_t const size) {
        RgResourceIdType const bufferId{ nextResourceId++ };
        buffers[bufferId] = std::make_unique<RgBuffer>(bufferId, std::move(buffer), size, offset);

        return bufferId;
    }

    void RenderGraph::writeToBuffer(RgResourceIdType buffer, void const *data, size_t const offset, size_t const size) {
        RgPassIdType const transferPassId{ nextPassId++ };

        auto &rgBuffer{ buffers.at(buffer) };
        rgBuffer->makeCpuAccessable();
        rgBuffer->addWritePass(transferPassId);

        RgTransferPass::BufferWrite write{
            .bufferId = buffer,
            .data     = std::vector<std::byte>(size),
            .offset   = offset,
            .size     = size,
        };
        memcpy(write.data.data(), data, size);

        transferPasses[transferPassId] = std::make_unique<RgTransferPass>(transferPassId, std::move(write));
    }

    RgResourceIdType RenderGraph::createImage(GhaImage::Type imageType, GhaImage::Format format, vec2ui dimensions) {
        RgResourceIdType const imageId{ nextResourceId++ };
        images[imageId] = std::make_unique<RgImage>(imageId, imageType, format, dimensions);

        return imageId;
    }

    RgResourceIdType RenderGraph::createImage(std::shared_ptr<GhaImageView> ghaImageView) {
        RgResourceIdType const imageId{ nextResourceId++ };
        images[imageId] = std::make_unique<RgImage>(imageId, std::move(ghaImageView));

        return imageId;
    }

    void RenderGraph::registerGraphOutput(RgResourceIdType resource) {
        outputResource = resource;
    }

    RgShader RenderGraph::createShader(std::filesystem::path const &file, GhaShader::Stage shaderStage) {
        RgShader const shader{ nextResourceId++ };
        allocatedShaders[shader] = globalCache.createShader(file, shaderStage);//Allocate straight away as it's usage does not define it's properties

        return shader;
    }

    RgShader RenderGraph::createShader(std::string_view source, std::unordered_map<std::string, std::string> includeSources, std::string_view shaderName, GhaShader::Stage shaderStage) {
        RgShader const shader{ nextResourceId++ };
        allocatedShaders[shader] = globalCache.createShader(source, std::move(includeSources), shaderName, shaderStage);//Allocate straight away as it's usage does not define it's properties

        return shader;
    }

    RgPassIdType RenderGraph::createRenderPass(RgRenderPass::Descriptor passDescriptor) {
        RgPassIdType const renderPassId{ nextPassId++ };

        for(auto &renderTarget : passDescriptor.renderTargets) {
            RgResourceIdType const imageId{ renderTarget.target };

            auto &image{ images.at(imageId) };
            if(!image->isExternalImage()) {
                image->addImageUsage(GhaImage::UsageMode::ColourAttachment);
            }
            image->addWritePass(renderPassId);
        }

        {
            RgResourceIdType const imageId{ passDescriptor.depthStencil.target };

            auto &image{ images.at(imageId) };
            if(!image->isExternalImage()) {
                image->addImageUsage(GhaImage::UsageMode::DepthStencilAttachment);
            }
            image->addWritePass(renderPassId);
        }

        renderPasses[renderPassId] = std::make_unique<RgRenderPass>(renderPassId, std::move(passDescriptor));

        return renderPassId;
    }

    void RenderGraph::addRenderSubmission(RgPassIdType const renderPass, RgRenderPass::Submission submission) {
        auto &pass{ renderPasses.at(renderPass) };

        {
            RgResourceIdType const bufferId{ submission.vertexBuffer };

            auto &buffer{ buffers.at(bufferId) };
            if(!buffer->isExternalBuffer()) {
                buffer->addBufferUsage(GhaBuffer::UsageMode::VertexBuffer);
            }
            buffer->addReadPass(renderPass);
        }

        {
            RgResourceIdType const bufferId{ submission.indexBuffer };

            auto &buffer{ buffers.at(bufferId) };
            if(!buffer->isExternalBuffer()) {
                buffer->addBufferUsage(GhaBuffer::UsageMode::IndexBuffer);
            }
            buffer->addReadPass(renderPass);
        }

        for(auto const &ubo : submission.shaderUbos) {
            RgResourceIdType const bufferId{ ubo.buffer };

            auto &buffer{ buffers.at(bufferId) };
            if(!buffer->isExternalBuffer()) {
                buffer->addBufferUsage(GhaBuffer::UsageMode::UniformBuffer);
            }
            buffer->addReadPass(renderPass);
        }

        for(auto const &imageSampler : submission.shaderCombinedImageSamplers) {
            RgResourceIdType const imageId{ imageSampler.image };

            auto &image{ images.at(imageId) };
            if(!image->isExternalImage()) {
                image->addImageUsage(GhaImage::UsageMode::Sampled);
            }
            image->addReadPass(renderPass);
        }

        pass->addSubmission(std::move(submission));
    }

    RgPassIdType RenderGraph::createComputePass(RgComputePass::Descriptor passDescriptor) {
        RgPassIdType const computePassId{ nextPassId++ };

        computePasses[computePassId] = std::make_unique<RgComputePass>(computePassId, std::move(passDescriptor));

        return computePassId;
    }

    void RenderGraph::addComputeSubmission(RgPassIdType const computePass, RgComputePass::Submission submission) {
        auto &pass{ computePasses.at(computePass) };

        for(auto const &buffer : submission.readUniformBuffers) {
            RgResourceIdType const bufferId{ buffer.buffer };

            auto &buffer{ buffers.at(bufferId) };
            if(!buffer->isExternalBuffer()) {
                buffer->addBufferUsage(GhaBuffer::UsageMode::UniformBuffer);
            }
            buffer->addReadPass(computePass);
        }

        for(auto const &buffer : submission.readStorageBuffers) {
            RgResourceIdType const bufferId{ buffer.buffer };

            auto &buffer{ buffers.at(bufferId) };
            if(!buffer->isExternalBuffer()) {
                buffer->addBufferUsage(GhaBuffer::UsageMode::StorageBuffer);
            }
            buffer->addReadPass(computePass);
        }

        for(auto const &buffer : submission.writeBuffers) {
            RgResourceIdType const bufferId{ buffer.buffer };

            auto &buffer{ buffers.at(bufferId) };
            if(!buffer->isExternalBuffer()) {
                buffer->addBufferUsage(GhaBuffer::UsageMode::StorageBuffer);
            }
            buffer->addWritePass(computePass);
        }

        pass->addSubmission(std::move(submission));
    }

    GraphicsSubmitInfo RenderGraph::execute() {
        CLOVE_ASSERT(outputResource != INVALID_RESOURCE_ID, "No output resource has been specified");

        //Build the array of passes to execute. Only adding passes which will ultimately effect the output resource.
        std::vector<RgPassIdType> executionPasses{};
        buildExecutionPasses(executionPasses, outputResource);

        //Filter out any duplicates and then reverse order so the output pass is at the end.
        {
            std::unordered_set<RgPassIdType> seenPasses{};
            for(auto iter{ executionPasses.begin() }; iter != executionPasses.end();) {
                if(!seenPasses.contains(*iter)) {
                    seenPasses.emplace(*iter);
                    ++iter;
                } else {
                    iter = executionPasses.erase(iter);
                }
            }
        }
        std::reverse(executionPasses.begin(), executionPasses.end());

        //Build a list of dependecies between the passes. These dependecies signify anything that needs a sempahore.
        std::vector<PassDependency> const passDependencies{ buildDependencies(executionPasses) };

        //Graphics GHA objects.
        std::unordered_map<RgPassIdType, std::shared_ptr<GhaRenderPass>> allocatedRenderPasses{};
        std::unordered_map<RgPassIdType, std::shared_ptr<GhaFramebuffer>> allocatedFramebuffers{};
        std::unordered_map<RgPassIdType, std::shared_ptr<GhaGraphicsPipelineObject>> allocatedGraphicsPipelines{};
        std::unordered_map<RgResourceIdType, std::shared_ptr<GhaSampler>> allocatedSamplers{};

        //Compute GHA objects.
        std::unordered_map<RgPassIdType, std::shared_ptr<GhaComputePipelineObject>> allocatedComputePipelines{};

        //Shared GHA objects.
        std::unordered_map<RgPassIdType, std::shared_ptr<GhaDescriptorSetLayout>> descriptorSetLayouts{};
        std::unordered_map<RgPassIdType, std::vector<std::shared_ptr<GhaDescriptorSet>>> allocatedDescriptorSets{};

        std::unordered_map<DescriptorType, uint32_t> totalDescriptorBindingCount{};
        uint32_t totalDescriptorSets{ 0 };

        //Allocate all of the GHA objects required for each pass
        generateRenderPassObjects(executionPasses, allocatedRenderPasses, allocatedFramebuffers, allocatedGraphicsPipelines, allocatedSamplers, descriptorSetLayouts, totalDescriptorBindingCount, totalDescriptorSets);
        generateComputePassObjects(executionPasses, allocatedComputePipelines, descriptorSetLayouts, totalDescriptorBindingCount, totalDescriptorSets);

        //Allocate descriptor sets
        allocatedDescriptorSets = createDescriptorSets(totalDescriptorBindingCount, totalDescriptorSets, allocatedGraphicsPipelines, allocatedComputePipelines);

        //Change sharing modes if objects are used in multiple queue types.
        //NOTE: Only doing buffers for now as images are yet to be supported
        //TODO: It might be better in the future to just insert the correct memory barriers.
        for(auto &&[bufferId, buffer] : buffers) {
            bool isInGraphics{ false };
            bool isInCompute{ false };

            for(RgPassIdType passId : buffer->getReadPasses()) {
                if(renderPasses.contains(passId)) {
                    isInGraphics = true;
                }
                if(computePasses.contains(passId)) {
                    isInCompute = true;
                }
            }

            for(RgPassIdType passId : buffer->getWritePasses()) {
                if(renderPasses.contains(passId)) {
                    isInGraphics = true;
                }
                if(computePasses.contains(passId)) {
                    isInCompute = true;
                }
            }

            if(isInGraphics && isInCompute) {
                buffer->setSharingMode(SharingMode::Concurrent);
            }
        }

        //Record all of the commands of each individual pass
        std::shared_ptr<GhaGraphicsCommandBuffer> graphicsCommandBufffer{ frameCache.allocateGraphicsCommandBuffer() };
        std::shared_ptr<GhaComputeCommandBuffer> computeCommandBufffer{ frameCache.allocateComputeCommandBuffer() };
        //std::shared_ptr<GhaTransferCommandBuffer> transferCommandBufffer{ frameCache.getTransferCommandBuffer() };

        //TODO: Handle each pass as a queue submission and handle them in order of a single iteration of the execution passes.

        //Handle all transfer work - This is fine for now as we don't use the transfer queue yet
        for(RgPassIdType passId : executionPasses) {
            if(!transferPasses.contains(passId)) {
                continue;
            }

            RgTransferPass::BufferWrite const &writeOp{ transferPasses.at(passId)->getWriteOperation() };
            std::unique_ptr<RgBuffer> const &buffer{ buffers.at(writeOp.bufferId) };
            buffer->getGhaBuffer(frameCache)->write(writeOp.data.data(), buffer->getBufferOffset() + writeOp.offset, writeOp.size);
        }

        //Handle all compute work first - This might cause issues with some work being blocked while other stuff waits on graphics work
        computeCommandBufffer->beginRecording(CommandBufferUsage::OneTimeSubmit);
        for(RgPassIdType passId : executionPasses) {
            if(!computePasses.contains(passId)) {
                continue;
            }

            RgComputePass::Descriptor const &passDescriptor{ computePasses.at(passId)->getDescriptor() };
            std::vector<RgComputePass::Submission> const &passSubmissions{ computePasses.at(passId)->getSubmissions() };

            computeCommandBufffer->bindPipelineObject(*allocatedComputePipelines.at(passId));

            for(size_t index{ 0 }; auto const &submission : passSubmissions) {
                std::shared_ptr<GhaDescriptorSet> const &descriptorSet{ allocatedDescriptorSets.at(passId)[index] };

                for(auto const &readUB : submission.readUniformBuffers) {
                    std::unique_ptr<RgBuffer> const &buffer{ buffers.at(readUB.buffer) };
                    descriptorSet->map(*buffer->getGhaBuffer(frameCache), 0, buffer->getBufferSize(), DescriptorType::UniformBuffer, readUB.slot);
                }
                for(auto const &readSB : submission.readStorageBuffers) {
                    std::unique_ptr<RgBuffer> const &buffer{ buffers.at(readSB.buffer) };
                    descriptorSet->map(*buffer->getGhaBuffer(frameCache), 0, buffer->getBufferSize(), DescriptorType::StorageBuffer, readSB.slot);
                }
                for(auto const &writeSB : submission.writeBuffers) {
                    std::unique_ptr<RgBuffer> const &buffer{ buffers.at(writeSB.buffer) };
                    descriptorSet->map(*buffer->getGhaBuffer(frameCache), 0, buffer->getBufferSize(), DescriptorType::StorageBuffer, writeSB.slot);
                }

                computeCommandBufffer->bindDescriptorSet(*descriptorSet, 0);

                computeCommandBufffer->disptach(submission.disptachSize);

                ++index;
            }
        }
        computeCommandBufffer->endRecording();

        std::vector<std::shared_ptr<GhaSemaphore>> computeSignalSemaphores{};
        for(auto const &dependency : passDependencies) {
            if(computePasses.contains(dependency.signalPass)) {
                computeSignalSemaphores.push_back(dependency.semaphore);
            }
        }

        frameCache.submit(ComputeSubmitInfo{
                              //TODO: Ignoring wait semaphores until submissions are handled internally properly
                              .commandBuffers   = { computeCommandBufffer },
                              .signalSemaphores = computeSignalSemaphores,
                          },
                          nullptr);

        //Handle all graphics work
        graphicsCommandBufffer->beginRecording(CommandBufferUsage::OneTimeSubmit);
        for(RgPassIdType passId : executionPasses) {
            if(!renderPasses.contains(passId)) {
                continue;
            }

            RgRenderPass::Descriptor const &passDescriptor{ renderPasses.at(passId)->getDescriptor() };
            std::vector<RgRenderPass::Submission> const &passSubmissions{ renderPasses.at(passId)->getSubmissions() };

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

            graphicsCommandBufffer->beginRenderPass(*allocatedRenderPasses.at(passId), *allocatedFramebuffers.at(passId), renderArea, clearValues);

            //TODO: Only do this if viewport is dynamic
            graphicsCommandBufffer->setViewport(passDescriptor.viewportPosition, passDescriptor.viewportSize);
            graphicsCommandBufffer->setScissor({ 0, 0 }, passDescriptor.viewportSize);

            graphicsCommandBufffer->bindPipelineObject(*allocatedGraphicsPipelines.at(passId));

            for(size_t index{ 0 }; auto const &submission : passSubmissions) {
                std::shared_ptr<GhaDescriptorSet> const &descriptorSet{ allocatedDescriptorSets.at(passId)[index] };

                for(auto const &ubo : submission.shaderUbos) {
                    //TODO: Handle different allocations within the same buffer
                    std::unique_ptr<RgBuffer> const &buffer{ buffers.at(ubo.buffer) };
                    descriptorSet->map(*buffer->getGhaBuffer(frameCache), 0, buffer->getBufferSize(), DescriptorType::UniformBuffer, ubo.slot);
                }
                for(auto const &imageSampler : submission.shaderCombinedImageSamplers) {
                    descriptorSet->map(*images.at(imageSampler.image)->getGhaImageView(frameCache), *allocatedSamplers.at(imageSampler.image), GhaImage::Layout::ShaderReadOnlyOptimal, imageSampler.slot);
                }

                graphicsCommandBufffer->bindDescriptorSet(*descriptorSet, 0);//TODO: Multiple sets / only set sets for a whole pass (i.e. view)

                std::unique_ptr<RgBuffer> const &vertexBuffer{ buffers.at(submission.vertexBuffer) };
                std::unique_ptr<RgBuffer> const &indexBuffer{ buffers.at(submission.indexBuffer) };
                graphicsCommandBufffer->bindVertexBuffer(*vertexBuffer->getGhaBuffer(frameCache), vertexBuffer->getBufferOffset());
                graphicsCommandBufffer->bindIndexBuffer(*indexBuffer->getGhaBuffer(frameCache), indexBuffer->getBufferOffset(), IndexType::Uint16);

                graphicsCommandBufffer->drawIndexed(submission.indexCount);

                ++index;
            }

            graphicsCommandBufffer->endRenderPass();
        }
        graphicsCommandBufffer->endRecording();

        std::vector<std::pair<std::shared_ptr<GhaSemaphore>, PipelineStage>> graphicsWaitSemaphores{};
        for(auto const &dependency : passDependencies) {
            if(renderPasses.contains(dependency.waitPass)) {
                graphicsWaitSemaphores.push_back(std::make_pair(dependency.semaphore, PipelineStage::Top));//TODO: Properly apply pipeline stage
            }
        }

        //TODO: Execute this internally
        return GraphicsSubmitInfo{
            .waitSemaphores = graphicsWaitSemaphores,
            .commandBuffers = { graphicsCommandBufffer },
            //TODO: Ignoring signal semaphores until submissions are handled properly
        };
    }

    void RenderGraph::buildExecutionPasses(std::vector<RgPassIdType> &outPasses, RgResourceIdType resourceId) {
        RgResource *resource{ getResourceFromId(resourceId) };

        std::vector<RgPassIdType> resourceWritePasses{};
        for(RgPassIdType passId : resource->getWritePasses()) {
            resourceWritePasses.push_back(passId);
        }

        outPasses.insert(outPasses.end(), resourceWritePasses.begin(), resourceWritePasses.end());

        for(RgPassIdType passId : resourceWritePasses) {
            RgPass *pass{ getPassFromId(passId) };
            for(RgResourceIdType resource : pass->getInputResources()) {
                buildExecutionPasses(outPasses, resource);
            }
        }
    }

    std::vector<RenderGraph::PassDependency> RenderGraph::buildDependencies(std::vector<RgPassIdType> const &passes) {
        std::vector<RenderGraph::PassDependency> dependencies{};

        auto const indexOf = [&passes](RgPassIdType passId) -> size_t {
            for(size_t i{ 0 }; i < passes.size(); ++i) {
                if(passes[i] == passId) {
                    return i;
                }
            }
            return -1;
        };

        //Dependecies are built for the closest pass that has an output of the current passes inputs. All queue operations complete in order so we only need to wait on the last one
        //First pass - create render pass dependencies.
        for(size_t passIndex{ 1 }; passIndex < passes.size(); ++passIndex) {
            RgPassIdType const passId{ passes[passIndex] };
            if(renderPasses.contains(passId)) {
                for(RgResourceIdType const resourceId : renderPasses.at(passId)->getInputResources()) {
                    RgResource const *const resource{ getResourceFromId(resourceId) };
                    RgPassIdType dependencyId{ 0 };
                    size_t currentDistance{ -1u };
                    bool hasDependency{ false };

                    for(RgPassIdType const dependencyPassId : resource->getWritePasses()) {
                        if(computePasses.contains(dependencyPassId)) {
                            size_t const dependencyIndex{ indexOf(dependencyPassId) };
                            if(dependencyIndex > passIndex) {
                                continue;
                            }

                            size_t const distance{ passId - dependencyIndex };
                            if(!hasDependency || distance < currentDistance) {
                                dependencyId    = dependencyPassId;
                                currentDistance = distance;
                                hasDependency   = true;
                            }
                        }
                    }

                    if(hasDependency) {
                        bool createDependency{ true };
                        //If a render pass before trhis one has a dependency then we don't need to wait because all queue submissions are done in order.
                        for(PassDependency const &dependency : dependencies) {
                            if(renderPasses.contains(dependency.waitPass) && indexOf(dependency.waitPass) < passIndex) {
                                createDependency = false;
                                break;
                            }
                        }

                        if(createDependency) {
                            dependencies.emplace_back(PassDependency{
                                .signalPass = dependencyId,
                                .waitPass   = passId,
                                .semaphore  = frameCache.allocateSemaphore(),
                            });
                        }
                    }
                }
            }
        }

        //Second pass - create compute pass dependencies.
        //TODO

        return dependencies;
    }

    GhaImage::Layout RenderGraph::getPreviousLayout(std::vector<RgPassIdType> const &passes, int32_t const currentPassIndex, RgResourceIdType const imageId) {
        for(int32_t i = currentPassIndex - 1; i >= 0; --i) {
            if(!renderPasses.contains(passes[i])) {
                continue;//Only evaluate renderpasses for now
            }
            std::unique_ptr<RgRenderPass> const &renderPass{ renderPasses.at(passes[i]) };

            if(renderPass->getInputResources().contains(imageId)) {
                for(auto const &submission : renderPass->getSubmissions()) {
                    for(auto const &imageSamplers : submission.shaderCombinedImageSamplers) {
                        if(imageSamplers.image == imageId) {
                            return GhaImage::Layout::ShaderReadOnlyOptimal;
                        }
                    }
                }

                CLOVE_ASSERT(false, "ImageId is not in any render target's submissions even though it's marked as an input resource");
            } else if(renderPass->getOutputResources().contains(imageId)) {
                for(auto const &renderTarget : renderPass->getDescriptor().renderTargets) {
                    if(renderTarget.target == imageId) {
                        return GhaImage::Layout::ColourAttachmentOptimal;
                    }
                }
                if(renderPass->getDescriptor().depthStencil.target == imageId) {
                    return GhaImage::Layout::ColourAttachmentOptimal;
                }

                CLOVE_ASSERT(false, "ImageId is not in any render target's output even though it's marked as an output resource");
            }
        }

        return GhaImage::Layout::Undefined;
    }

    RgResource *RenderGraph::getResourceFromId(RgResourceIdType resourceId) {
        if(images.contains(resourceId)) {
            return images.at(resourceId).get();
        } else if(buffers.contains(resourceId)) {
            return buffers.at(resourceId).get();
        } else {
            return nullptr;
        }
    }

    RgPass *RenderGraph::getPassFromId(RgPassIdType passId) {
        if(renderPasses.contains(passId)) {
            return renderPasses.at(passId).get();
        } else if(computePasses.contains(passId)) {
            return computePasses.at(passId).get();
        } else if(transferPasses.contains(passId)) {
            return transferPasses.at(passId).get();
        } else {
            CLOVE_ASSERT(false, "Could not find pass ID");
            return nullptr;
        }
    }

    void RenderGraph::generateRenderPassObjects(std::vector<RgPassIdType> const &passes, std::unordered_map<RgPassIdType, std::shared_ptr<GhaRenderPass>> &outRenderPasses, std::unordered_map<RgPassIdType, std::shared_ptr<GhaFramebuffer>> &outFramebuffers, std::unordered_map<RgPassIdType, std::shared_ptr<GhaGraphicsPipelineObject>> &outGraphicsPipelines, std::unordered_map<RgResourceIdType, std::shared_ptr<GhaSampler>> &outSamplers, std::unordered_map<RgPassIdType, std::shared_ptr<GhaDescriptorSetLayout>> &outDescriptorSetLayouts, std::unordered_map<DescriptorType, uint32_t> &totalDescriptorBindingCount, uint32_t &totalDescriptorSets) {
        for(int32_t i{ 0 }; i < passes.size(); ++i) {
            RgPassIdType const passId{ passes[i] };
            if(!renderPasses.contains(passId)) {
                continue;
            }

            RgRenderPass::Descriptor const &passDescriptor{ renderPasses.at(passId)->getDescriptor() };
            std::vector<RgRenderPass::Submission> const &passSubmissions{ renderPasses.at(passId)->getSubmissions() };

            //Allocate image samplers
            for(auto const &submission : passSubmissions) {
                for(auto const &imageSampler : submission.shaderCombinedImageSamplers) {
                    outSamplers[passId] = globalCache.createSampler(imageSampler.samplerState);
                }
            }

            //Build and allocate the render pass
            std::vector<AttachmentDescriptor> colourAttachments{};
            for(auto &renderTarget : passDescriptor.renderTargets) {
                colourAttachments.emplace_back(AttachmentDescriptor{
                    .format         = images[renderTarget.target]->getGhaImageView(frameCache)->getImageFormat(),
                    .loadOperation  = renderTarget.loadOp,
                    .storeOperation = renderTarget.storeOp,
                    .initialLayout  = getPreviousLayout(passes, i, renderTarget.target),
                    .usedLayout     = GhaImage::Layout::ColourAttachmentOptimal,
                    .finalLayout    = renderTarget.target == outputResource ? GhaImage::Layout::Present : GhaImage::Layout::ColourAttachmentOptimal,
                });
            }

            RgDepthStencilBinding const &depthStencil{ passDescriptor.depthStencil };
            AttachmentDescriptor depthStencilAttachment{
                .format         = images[depthStencil.target]->getGhaImageView(frameCache)->getImageFormat(),
                .loadOperation  = depthStencil.loadOp,
                .storeOperation = depthStencil.storeOp,
                .initialLayout  = getPreviousLayout(passes, i, depthStencil.target),
                .usedLayout     = GhaImage::Layout::DepthStencilAttachmentOptimal,
                .finalLayout    = GhaImage::Layout::DepthStencilAttachmentOptimal,
            };

            outRenderPasses[passId] = globalCache.createRenderPass(GhaRenderPass::Descriptor{ .colourAttachments = std::move(colourAttachments), .depthAttachment = std::move(depthStencilAttachment) });

            //Build descriptor layouts using the first pass.
            //TODO: Get this infomation from shader reflection
            std::vector<DescriptorSetBindingInfo> descriptorBindings{};
            for(auto &ubo : passSubmissions[0].shaderUbos) {
                descriptorBindings.emplace_back(DescriptorSetBindingInfo{
                    .binding   = ubo.slot,
                    .type      = DescriptorType::UniformBuffer,
                    .arraySize = 1,
                    .stage     = GhaShader::Stage::Vertex,//TODO: provided by pass or by shader reflection
                });
            }
            for(auto &sampler : passSubmissions[0].shaderCombinedImageSamplers) {
                descriptorBindings.emplace_back(DescriptorSetBindingInfo{
                    .binding   = sampler.slot,
                    .type      = DescriptorType::CombinedImageSampler,
                    .arraySize = 1,
                    .stage     = GhaShader::Stage::Pixel,//TODO: provided by pass or by shader reflection
                });
            }

            outDescriptorSetLayouts[passId] = globalCache.createDescriptorSetLayout(GhaDescriptorSetLayout::Descriptor{ .bindings = std::move(descriptorBindings) });

            //Build graphics pipeline
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

            outGraphicsPipelines[passId] = globalCache.createGraphicsPipelineObject(GhaGraphicsPipelineObject::Descriptor{
                .vertexShader       = allocatedShaders.at(passDescriptor.vertexShader),
                .pixelShader        = allocatedShaders.at(passDescriptor.pixelShader),
                .vertexInput        = Vertex::getInputBindingDescriptor(),
                .vertexAttributes   = vertexAttributes,
                .viewportDescriptor = viewScissorArea,
                .scissorDescriptor  = viewScissorArea,
                .depthState         = {
                    .depthTest  = passDescriptor.depthTest,
                    .depthWrite = passDescriptor.depthWrite,
                },
                .enableBlending       = passDescriptor.enableBlending,
                .renderPass           = outRenderPasses.at(passId),
                .descriptorSetLayouts = { outDescriptorSetLayouts.at(passId) },
                .pushConstants        = {},
            });

            //Allocate the frame buffer
            std::vector<std::shared_ptr<GhaImageView>> attachments{};
            for(auto &renderTarget : passDescriptor.renderTargets) {
                attachments.push_back(images.at(renderTarget.target)->getGhaImageView(frameCache));
            }
            attachments.push_back(images.at(passDescriptor.depthStencil.target)->getGhaImageView(frameCache));

            outFramebuffers[passId] = frameCache.allocateFramebuffer(GhaFramebuffer::Descriptor{
                .renderPass  = outRenderPasses.at(passId),
                .attachments = std::move(attachments),
                .width       = images[passDescriptor.renderTargets[0].target]->getGhaImageView(frameCache)->getImageDimensions().x,//TEMP: Just using the first target as the size. This will need to be validated
                .height      = images[passDescriptor.renderTargets[0].target]->getGhaImageView(frameCache)->getImageDimensions().y,
            });

            //Count descriptor sets required for the entire pass
            for(auto const &submission : passSubmissions) {
                bool const hasUbo{ !submission.shaderUbos.empty() };
                bool const hasImageSampler{ !submission.shaderCombinedImageSamplers.empty() };

                if(hasUbo) {
                    totalDescriptorBindingCount[DescriptorType::UniformBuffer] += submission.shaderUbos.size();
                }
                if(hasImageSampler) {
                    totalDescriptorBindingCount[DescriptorType::CombinedImageSampler] += submission.shaderCombinedImageSamplers.size();
                }

                if(hasUbo || hasImageSampler) {
                    ++totalDescriptorSets;//Allocating a single set per submission
                }
            }
        }
    }

    void RenderGraph::generateComputePassObjects(std::vector<RgPassIdType> const &passes, std::unordered_map<RgPassIdType, std::shared_ptr<GhaComputePipelineObject>> &outComputePipelines, std::unordered_map<RgPassIdType, std::shared_ptr<GhaDescriptorSetLayout>> &outDescriptorSetLayouts, std::unordered_map<DescriptorType, uint32_t> &totalDescriptorBindingCount, uint32_t &totalDescriptorSets) {
        for(int32_t i{ 0 }; i < passes.size(); ++i) {
            RgPassIdType const passId{ passes[i] };
            if(!computePasses.contains(passId)) {
                continue;
            }

            RgComputePass::Descriptor const &passDescriptor{ computePasses.at(passId)->getDescriptor() };
            std::vector<RgComputePass::Submission> const &passSubmissions{ computePasses.at(passId)->getSubmissions() };

            //Build descriptor layouts using the first pass.
            //TODO: Get this infomation from shader reflection
            std::vector<DescriptorSetBindingInfo> descriptorBindings{};
            for(auto &ubo : passSubmissions[0].readUniformBuffers) {
                descriptorBindings.emplace_back(DescriptorSetBindingInfo{
                    .binding   = ubo.slot,
                    .type      = DescriptorType::UniformBuffer,
                    .arraySize = 1,
                    .stage     = GhaShader::Stage::Compute,
                });
            }
            for(auto &sbo : passSubmissions[0].readStorageBuffers) {
                descriptorBindings.emplace_back(DescriptorSetBindingInfo{
                    .binding   = sbo.slot,
                    .type      = DescriptorType::StorageBuffer,
                    .arraySize = 1,
                    .stage     = GhaShader::Stage::Compute,
                });
            }
            for(auto &sbo : passSubmissions[0].writeBuffers) {
                descriptorBindings.emplace_back(DescriptorSetBindingInfo{
                    .binding   = sbo.slot,
                    .type      = DescriptorType::StorageBuffer,
                    .arraySize = 1,
                    .stage     = GhaShader::Stage::Compute,
                });
            }

            outDescriptorSetLayouts[passId] = globalCache.createDescriptorSetLayout(GhaDescriptorSetLayout::Descriptor{ .bindings = std::move(descriptorBindings) });

            //Build compute pipeline
            outComputePipelines[passId] = globalCache.createComputePipelineObject(GhaComputePipelineObject::Descriptor{
                .shader               = allocatedShaders.at(passDescriptor.shader),
                .descriptorSetLayouts = { outDescriptorSetLayouts.at(passId) },
                .pushConstants        = {},
            });

            //Count descriptor sets required for the entire pass
            for(auto const &submission : passSubmissions) {
                bool const hasUbo{ !submission.readUniformBuffers.empty() };
                bool const hasSbo{ !submission.readStorageBuffers.empty() };
                bool const hasWriteBuffer{ !submission.writeBuffers.empty() };

                if(hasUbo) {
                    totalDescriptorBindingCount[DescriptorType::UniformBuffer] += submission.readUniformBuffers.size();
                }
                if(hasSbo) {
                    totalDescriptorBindingCount[DescriptorType::StorageBuffer] += submission.readStorageBuffers.size();
                }
                if(hasWriteBuffer) {
                    totalDescriptorBindingCount[DescriptorType::StorageBuffer] += submission.writeBuffers.size();
                }

                if(hasUbo || hasSbo || hasWriteBuffer) {
                    ++totalDescriptorSets;//Allocating a single set per submission
                }
            }
        }
    }

    std::unordered_map<RgPassIdType, std::vector<std::shared_ptr<GhaDescriptorSet>>> RenderGraph::createDescriptorSets(std::unordered_map<DescriptorType, uint32_t> const &totalDescriptorBindingCount, uint32_t const totalDescriptorSets, std::unordered_map<RgPassIdType, std::shared_ptr<GhaGraphicsPipelineObject>> const &graphicsPipelines, std::unordered_map<RgPassIdType, std::shared_ptr<GhaComputePipelineObject>> &computePipelines) {
        std::unordered_map<RgPassIdType, std::vector<std::shared_ptr<GhaDescriptorSet>>> descriptorSets{};

        //Create a single pool to allocate from
        std::vector<DescriptorInfo> descriptorTypes{};
        descriptorTypes.reserve(totalDescriptorBindingCount.size());

        for(auto &&[type, count] : totalDescriptorBindingCount) {
            descriptorTypes.emplace_back(DescriptorInfo{
                .type  = type,
                .count = count,
            });
        }

        std::shared_ptr<GhaDescriptorPool> descriptorPool{ frameCache.allocateDescriptorPool(GhaDescriptorPool::Descriptor{
            .poolTypes = std::move(descriptorTypes),
            .flag      = GhaDescriptorPool::Flag::None,
            .maxSets   = totalDescriptorSets,
        }) };
        descriptorPool->reset();//Make sure we have a fresh pool. TODO: do this inside frame cache? Or have a function to reset states

        //Create a descriptor set for each pipeline
        for(auto &&[id, pipeline] : graphicsPipelines) {
            std::vector<std::shared_ptr<GhaDescriptorSetLayout>> layouts(renderPasses.at(id)->getSubmissions().size(), pipeline->getDescriptor().descriptorSetLayouts[0]);//TEMP: Using first index as we know pipelines always have a single descriptor for now
            descriptorSets[id] = descriptorPool->allocateDescriptorSets(layouts);
        }
        for(auto &&[id, pipeline] : computePipelines) {
            std::vector<std::shared_ptr<GhaDescriptorSetLayout>> layouts(computePasses.at(id)->getSubmissions().size(), pipeline->getDescriptor().descriptorSetLayouts[0]);//TEMP: Using first index as we know pipelines always have a single descriptor for now
            descriptorSets[id] = descriptorPool->allocateDescriptorSets(layouts);
        }

        return descriptorSets;
    }
}