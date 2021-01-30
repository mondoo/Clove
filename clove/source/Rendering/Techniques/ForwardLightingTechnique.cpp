#include "Clove/Rendering/Techniques/ForwardLightingTechnique.hpp"

#include "Clove/Rendering/RenderPasses/ForwardColourPass.hpp"

namespace garlic::clove {
    Technique createForwardLightingTechnique() {
        return { std::set<GeometryPass::Id>{ GeometryPass::getId<ForwardColourPass>() } };
    }
}