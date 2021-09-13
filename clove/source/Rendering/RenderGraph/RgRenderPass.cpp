#include "Clove/Rendering/RenderGraph/RgRenderPass.hpp"

#include "Clove/Rendering/RenderGraph/RgGlobalCache.hpp"

namespace clove {
    RgRenderPass::RgRenderPass(RgPassId id, Descriptor descriptor)
        : RgPass{ id }
        , descriptor{ std::move(descriptor) } {
    }

    RgRenderPass::RgRenderPass(RgRenderPass &&other) noexcept = default;

    RgRenderPass &RgRenderPass::operator=(RgRenderPass &&other) noexcept = default;

    RgRenderPass::~RgRenderPass() = default;

    std::unordered_set<RgResourceId> RgRenderPass::getInputResources() const {
        std::unordered_set<RgResourceId> inputResources{};
        for(auto const &submission : submissions) {
            inputResources.emplace(submission.vertexBuffer);
            inputResources.emplace(submission.indexBuffer);
            for(auto const &ubo : submission.shaderUbos){
                inputResources.emplace(ubo.buffer);
            }
            for(auto const &image : submission.shaderImages){
                inputResources.emplace(image.image);
            }
        }
        return inputResources;
    }

    std::unordered_set<RgResourceId> RgRenderPass::getOutputResources() const {
        std::unordered_set<RgResourceId> outputResources{};
        for(auto const &renderTarget : descriptor.renderTargets){
            outputResources.emplace(renderTarget.target);
        }
        outputResources.emplace(descriptor.depthStencil.target);
        return outputResources;
    }
}