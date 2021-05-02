#include "Clove/Rendering/RenderGraph/RgGraphicsPipelineState.hpp"

namespace garlic::clove {
    RgGraphicsPipelineState::RgGraphicsPipelineState() = default;

    RgGraphicsPipelineState::RgGraphicsPipelineState(RgGraphicsPipelineState const &other) = default;
    RgGraphicsPipelineState::RgGraphicsPipelineState(RgGraphicsPipelineState &&other) noexcept = default;

    RgGraphicsPipelineState &RgGraphicsPipelineState::operator=(RgGraphicsPipelineState const &other) = default;
    RgGraphicsPipelineState &RgGraphicsPipelineState::operator=(RgGraphicsPipelineState &&other) noexcept = default;

    RgGraphicsPipelineState::~RgGraphicsPipelineState() = default;

    RgGraphicsPipelineState::RgGraphicsPipelineState(ResourceIdType id)
        : id{ id } {
    }
}