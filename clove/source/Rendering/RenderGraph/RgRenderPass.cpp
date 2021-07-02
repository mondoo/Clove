#include "Clove/Rendering/RenderGraph/RgRenderPass.hpp"

namespace garlic::clove {
    RgRenderPass::RgRenderPass(PassIdType id, Descriptor descriptor)
        : RgPass{ id }
        , descriptor{ std::move(descriptor) } {
    }

    RgRenderPass::RgRenderPass(RgRenderPass &&other) noexcept = default;

    RgRenderPass &RgRenderPass::operator=(RgRenderPass &&other) noexcept = default;

    RgRenderPass::~RgRenderPass() = default;
}