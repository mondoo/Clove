#pragma once

#include "Clove/Rendering/RenderPasses/GeometryPass.hpp"

#include <memory>

namespace clove {
    class GhaFactory;
    class GhaGraphicsPipelineObject;
    class GhaRenderPass;
}

namespace clove {
    class DirectionalLightPass : public GeometryPass {
        //FUNCTIONS
    public:
        DirectionalLightPass();

        DirectionalLightPass(DirectionalLightPass const &other) = delete;
        DirectionalLightPass(DirectionalLightPass &&other) noexcept;

        DirectionalLightPass &operator=(DirectionalLightPass const &other) = delete;
        DirectionalLightPass &operator=(DirectionalLightPass &&other) noexcept;

        ~DirectionalLightPass();

        void execute(RenderGraph &renderGraph, PassData const &passData) override;
    };
}