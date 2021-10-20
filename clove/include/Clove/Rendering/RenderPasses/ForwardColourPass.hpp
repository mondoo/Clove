#pragma once

#include "Clove/Rendering/RenderPasses/GeometryPass.hpp"

#include <memory>

namespace clove {
    class GhaFactory;
    class GhaGraphicsPipelineObject;
    class GhaRenderPass;
}

namespace clove {
    class ForwardColourPass : public GeometryPass {
        //FUNCTIONS
    public:
        ForwardColourPass();

        ForwardColourPass(ForwardColourPass const &other) = delete;
        ForwardColourPass(ForwardColourPass &&other) noexcept;

        ForwardColourPass &operator=(ForwardColourPass const &other) = delete;
        ForwardColourPass &operator=(ForwardColourPass &&other) noexcept;

        ~ForwardColourPass();

        void execute(RenderGraph &renderGraph, PassData const &passData) override;
    };
}