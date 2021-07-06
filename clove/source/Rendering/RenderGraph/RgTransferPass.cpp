#include "Clove/Rendering/RenderGraph/RgTransferPass.hpp"

namespace garlic::clove {
    RgTransferPass::RgTransferPass(PassIdType id, BufferWrite writePass)
        : RgPass{ id }
        , writePass{ std::move(writePass) } {
    }

    RgTransferPass::RgTransferPass(RgTransferPass &&other) noexcept = default;

    RgTransferPass &RgTransferPass::operator=(RgTransferPass &&other) noexcept = default;

    RgTransferPass::~RgTransferPass() = default;

    std::unordered_set<ResourceIdType> RgTransferPass::getInputResources() const {
        return {};
    }

    std::unordered_set<ResourceIdType> RgTransferPass::getOutputResources() const {
        return { writePass.bufferId };
    }
}