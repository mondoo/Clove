#pragma once

#include "Clove/Rendering/RenderPasses/GeometryPass.hpp"

namespace garlic::clove {
    class SkinningPass : public GeometryPass {
        //FUNCTIONS
    public:
        void execute(GhaGraphicsCommandBuffer &commandBuffer, FrameData const &frameData) override;
    };
}