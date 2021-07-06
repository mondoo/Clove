#include "Clove/Rendering/RenderGraph/RenderGraph.hpp"

#include "Clove/Rendering/RenderGraph/RgFrameCache.hpp"
#include "Clove/Rendering/RenderGraph/RgGlobalCache.hpp"
#include "Clove/Rendering/Vertex.hpp"

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

    ResourceIdType RenderGraph::createBuffer(size_t const bufferSize) {
        ResourceIdType const bufferId{ nextResourceId++ };
        buffers[bufferId] = std::make_unique<RgBuffer>(bufferId, bufferSize);

        return bufferId;
    }

    ResourceIdType RenderGraph::createBuffer(std::shared_ptr<GhaBuffer> buffer, size_t const offset, size_t const size) {
        ResourceIdType const bufferId{ nextResourceId++ };
        buffers[bufferId] = std::make_unique<RgBuffer>(bufferId, std::move(buffer), size, offset);

        return bufferId;
    }

    void RenderGraph::writeToBuffer(ResourceIdType buffer, void const *data, size_t const offset, size_t const size) {
        //TODO: Add a special pass type for transfers
    }

    ResourceIdType RenderGraph::createImage(GhaImage::Type imageType, GhaImage::Format format, vec2ui dimensions) {
        ResourceIdType const imageId{ nextResourceId++ };
        images[imageId] = std::make_unique<RgImage>(imageId, imageType, format, dimensions);

        return imageId;
    }

    ResourceIdType RenderGraph::createImage(std::shared_ptr<GhaImageView> ghaImageView) {
        ResourceIdType const imageId{ nextResourceId++ };
        images[imageId] = std::make_unique<RgImage>(imageId, std::move(ghaImageView));

        return imageId;
    }

    void RenderGraph::registerGraphOutput(ResourceIdType resource) {
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

    PassIdType RenderGraph::addRenderPass(RgRenderPass::Descriptor passDescriptor) {
        PassIdType const renderPassId{ nextPassId };

        for(auto &renderTarget : passDescriptor.renderTargets) {
            ResourceIdType const imageId{ renderTarget.target };

            auto &image{ images.at(imageId) };
            image->addImageUsage(GhaImage::UsageMode::ColourAttachment);
            image->addWritePass(renderPassId);
        }

        {
            ResourceIdType const imageId{ passDescriptor.depthStencil.target };

            auto &image{ images.at(imageId) };
            image->addImageUsage(GhaImage::UsageMode::DepthStencilAttachment);
            image->addWritePass(renderPassId);
        }

        renderPasses[renderPassId] = std::make_unique<RgRenderPass>(renderPassId, std::move(passDescriptor));

        return renderPassId;
    }

    void RenderGraph::addRenderSubmission(PassIdType const renderPass, RgRenderPass::Submission submission) {
        auto &pass{ renderPasses.at(renderPass) };

        {
            ResourceIdType const bufferId{ submission.vertexBuffer };

            auto &buffer{ buffers.at(bufferId) };
            buffer->addBufferUsage(GhaBuffer::UsageMode::VertexBuffer);
            buffer->addReadPass(renderPass);
        }

        {
            ResourceIdType const bufferId{ submission.indexBuffer };

            auto &buffer{ buffers.at(bufferId) };
            buffer->addBufferUsage(GhaBuffer::UsageMode::IndexBuffer);
            buffer->addReadPass(renderPass);
        }

        for(auto const &ubo : submission.shaderUbos) {
            ResourceIdType const bufferId{ ubo.buffer };

            auto &buffer{ buffers.at(bufferId) };
            buffer->addBufferUsage(GhaBuffer::UsageMode::UniformBuffer);
            buffer->addReadPass(renderPass);
        }

        for(auto const &imageSampler : submission.shaderCombinedImageSamplers) {
            ResourceIdType const imageId{ imageSampler.image };

            auto &image{ images.at(imageId) };
            image->addImageUsage(GhaImage::UsageMode::Sampled);
            image->addReadPass(renderPass);
        }

        pass->addSubmission(std::move(submission));
    }

    GraphicsSubmitInfo RenderGraph::execute() {
        CLOVE_ASSERT(outputResource != INVALID_RESOURCE_ID, "No output resource has been specified");

        //Build an array of passes. This will be the execution order.
        std::vector<PassIdType> passes{};
        buildExecutionPasses(passes, outputResource);

        //Filter out any duplicates and then reverse order so out output pass is at the end
        {
            std::unordered_set<PassIdType> seenPasses{};
            for(auto iter{ passes.begin() }; iter != passes.end();) {
                if(!seenPasses.contains(*iter)) {
                    seenPasses.emplace(*iter);
                    ++iter;
                } else {
                    iter = passes.erase(iter);
                }
            }
        }
        std::reverse(passes.begin(), passes.end());

        //Allocate all of the GHA objects required for each pass
        std::unordered_map<PassIdType, std::shared_ptr<GhaRenderPass>> allocatedRenderPasses{};
        std::unordered_map<PassIdType, std::shared_ptr<GhaFramebuffer>> allocatedFramebuffers{};
        std::unordered_map<PassIdType, std::shared_ptr<GhaGraphicsPipelineObject>> allocatedPipelines{};
        std::unordered_map<PassIdType, std::shared_ptr<GhaDescriptorSetLayout>> descriptorSetLayouts{};
        std::unordered_map<PassIdType, std::vector<std::shared_ptr<GhaDescriptorSet>>> allocatedDescriptorSets{};

        for(int32_t i{ 0 }; i < passes.size(); ++i) {
            PassIdType const passId{ passes[i] };

            //Build and allocate the render pass
            std::vector<AttachmentDescriptor> colourAttachments{};
            for(auto &renderTarget : renderPasses.at(passId)->getDescriptor().renderTargets) {
                colourAttachments.emplace_back(AttachmentDescriptor{
                    .format         = images[renderTarget.target]->getGhaImageView(frameCache)->getImageFormat(),
                    .loadOperation  = renderTarget.loadOp,
                    .storeOperation = renderTarget.storeOp,
                    .initialLayout  = getPreviousLayout(passes, i, renderTarget.target),
                    .usedLayout     = GhaImage::Layout::ColourAttachmentOptimal,
                    .finalLayout    = renderTarget.target == outputResource ? GhaImage::Layout::Present : GhaImage::Layout::ColourAttachmentOptimal,
                });
            }

            DepthStencilBinding const &depthStencil{ renderPasses.at(passId)->getDescriptor().depthStencil };
            AttachmentDescriptor depthStencilAttachment{
                .format         = images[depthStencil.target]->getGhaImageView(frameCache)->getImageFormat(),
                .loadOperation  = depthStencil.loadOp,
                .storeOperation = depthStencil.storeOp,
                .initialLayout  = getPreviousLayout(passes, i, depthStencil.target),
                .usedLayout     = GhaImage::Layout::DepthStencilAttachmentOptimal,
                .finalLayout    = GhaImage::Layout::DepthStencilAttachmentOptimal,
            };

            allocatedRenderPasses[passId] = globalCache.createRenderPass(GhaRenderPass::Descriptor{ .colourAttachments = std::move(colourAttachments), .depthAttachment = std::move(depthStencilAttachment) });
        }

        //Record all of the commands of each individual pass
        std::shared_ptr<GhaGraphicsCommandBuffer> graphicsCommandBufffer{ frameCache.getGraphicsCommandBuffer() };
        //std::shared_ptr<GhaComputeCommandBuffer> computeCommandBufffer{ frameCache.getComputeCommandBuffer() };
        //std::shared_ptr<GhaTransferCommandBuffer> transferCommandBufffer{ frameCache.getTransferCommandBuffer() };

        graphicsCommandBufffer->beginRecording(CommandBufferUsage::OneTimeSubmit);
        for(PassIdType passId : passes) {
            //TODO
        }
        graphicsCommandBufffer->endRecording();

        return {};
    }

    void RenderGraph::buildExecutionPasses(std::vector<PassIdType> &passes, ResourceIdType resourceId) {
        RgResource *resource{ getResourceFromId(resourceId) };

        std::vector<PassIdType> resourceWritePasses{};
        for(PassIdType passId : resource->getWritePasses()) {
            resourceWritePasses.push_back(passId);
        }

        passes.insert(passes.end(), resourceWritePasses.begin(), resourceWritePasses.end());

        for(PassIdType passId : resourceWritePasses) {
            RgPass *pass{ getPassFromId(passId) };
            for(ResourceIdType resource : pass->getInputResources()) {
                buildExecutionPasses(passes, resource);
            }
        }
    }

    GhaImage::Layout RenderGraph::getPreviousLayout(std::vector<PassIdType> const &passes, int32_t const currentPassIndex, ResourceIdType const imageId) {
        for(int32_t i = currentPassIndex - 1; i >= 0; --i) {
            std::unique_ptr<RgRenderPass> const &renderPass{ renderPasses[passes[i]] };

            if(renderPass->getInputResources().contains(imageId)) {
                for(auto const &submission : renderPass->getSubmissions()){
                    for(auto const &imageSamplers : submission.shaderCombinedImageSamplers) {
                        if(imageSamplers.image == imageId){
                            return GhaImage::Layout::ShaderReadOnlyOptimal;
                        }
                    }
                }

                CLOVE_ASSERT(false, "ImageId is not in any render target's submissions even though it's marked as an input resource");
            } else if(renderPass->getOutputResources().contains(imageId)){
                for(auto const &renderTarget : renderPass->getDescriptor().renderTargets){
                    if(renderTarget.target == imageId){
                        return GhaImage::Layout::ColourAttachmentOptimal;
                    }
                }
                if(renderPass->getDescriptor().depthStencil.target == imageId){
                    return GhaImage::Layout::ColourAttachmentOptimal;
                }

                CLOVE_ASSERT(false, "ImageId is not in any render target's output even though it's marked as an output resource");
            }
        }

        return GhaImage::Layout::Undefined;
    }

    RgResource *RenderGraph::getResourceFromId(ResourceIdType resourceId) {
        if(images.contains(resourceId)) {
            return images.at(resourceId).get();
        } else if(buffers.contains(resourceId)) {
            return buffers.at(resourceId).get();
        } else {
            return nullptr;
        }
    }

    RgPass *RenderGraph::getPassFromId(PassIdType passId) {
        if(renderPasses.contains(passId)) {
            return renderPasses.at(passId).get();
        } else {
            return nullptr;
        }
    }
}