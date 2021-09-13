#include "Clove/Rendering/RenderGraph/RgPass.hpp"

namespace clove {
    RgPass::RgPass(RgPassId id)
        : id{ id } {
    }

    RgPass::RgPass(RgPass &&other) noexcept = default;

    RgPass &RgPass::operator=(RgPass &&other) noexcept = default;

    RgPass::~RgPass() = default;
}