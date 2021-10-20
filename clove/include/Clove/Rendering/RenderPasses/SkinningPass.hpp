#pragma once

#include "Clove/Rendering/RenderPasses/GeometryPass.hpp"

#include <memory>

namespace clove {
    class GhaComputePipelineObject;
    class GhaFactory;
}

namespace clove {
    class SkinningPass : public GeometryPass {
        //FUNCTIONS
    public:
        SkinningPass();

        SkinningPass(SkinningPass const &other) = delete;
        SkinningPass(SkinningPass &&other) noexcept;

        SkinningPass &operator=(SkinningPass const &other) = delete;
        SkinningPass &operator=(SkinningPass &&other) noexcept;

        ~SkinningPass();

        void execute(RenderGraph &renderGraph, PassData const &passData) override;
    };
}