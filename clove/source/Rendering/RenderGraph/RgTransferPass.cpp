#include "Clove/Rendering/RenderGraph/RgTransferPass.hpp"

namespace clove {
    RgTransferPass::RgTransferPass(RgPassId id, BufferWrite writePass)
        : RgPass{ id }
        , writePass{ std::move(writePass) } {
    }

    RgTransferPass::RgTransferPass(RgTransferPass &&other) noexcept = default;

    RgTransferPass &RgTransferPass::operator=(RgTransferPass &&other) noexcept = default;

    RgTransferPass::~RgTransferPass() = default;

    std::unordered_set<RgResourceId> RgTransferPass::getInputResources() const {
        return {};
    }

    std::unordered_set<RgResourceId> RgTransferPass::getOutputResources() const {
        return { writePass.bufferId };
    }
}