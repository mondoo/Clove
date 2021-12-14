#include "Clove/Rendering/Techniques/ForwardLightingTechnique.hpp"

#include "Clove/Rendering/RenderPasses/ForwardColourPass.hpp"
#include "Clove/Rendering/RenderPasses/DirectionalLightPass.hpp"
#include "Clove/Rendering/RenderPasses/PointLightPass.hpp"
#include "Clove/Rendering/RenderPasses/SkinningPass.hpp"

namespace clove {
    Technique createForwardLightingTechnique() {
        return Technique{
            .passIds = std::set<GeometryPass::Id>{
                GeometryPass::getIdOf<ForwardColourPass>(),
                GeometryPass::getIdOf<DirectionalLightPass>(),
                GeometryPass::getIdOf<PointLightPass>()
            },
        };
    }

    Technique createSkinnedForwardLightingTechnique(){
        Technique technique{ createForwardLightingTechnique() };
        technique.passIds.emplace(GeometryPass::getIdOf<SkinningPass>());

        return technique;
    }
}
