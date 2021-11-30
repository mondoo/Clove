#pragma once

#include "Clove/Rendering/RenderPasses/GeometryPass.hpp"

#include <memory>

namespace clove {
    class GhaFactory;
    class GhaGraphicsPipelineObject;
    class GhaRenderPass;
}

namespace clove {
    class PointLightPass : public GeometryPass {
        //FUNCTIONS
    public:
        PointLightPass();

        PointLightPass(PointLightPass const &other) = delete;
        PointLightPass(PointLightPass &&other) noexcept;

        PointLightPass &operator=(PointLightPass const &other) = delete;
        PointLightPass &operator=(PointLightPass &&other) noexcept;

        ~PointLightPass();
        
        Id getId() const override;

        void execute(RenderGraph &renderGraph, PassData const &passData) override;
    };
}
