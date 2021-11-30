#include "Clove/Rendering/RenderPasses/PointLightPass.hpp"

extern "C" const char constants[];
extern "C" const size_t constantsLength;

extern "C" const char meshcubeshadowmap_v[];
extern "C" const size_t meshcubeshadowmap_vLength;
extern "C" const char meshcubeshadowmap_p[];
extern "C" const size_t meshcubeshadowmap_pLength;

namespace clove {
    PointLightPass::PointLightPass() = default;

    PointLightPass::PointLightPass(PointLightPass &&other) noexcept = default;

    PointLightPass &PointLightPass::operator=(PointLightPass &&other) noexcept = default;

    PointLightPass::~PointLightPass() = default;

    GeometryPass::Id PointLightPass::getId() const {
        return getIdOf<PointLightPass>();
    }
    
    void PointLightPass::execute(RenderGraph &renderGraph, PassData const &passData) {
        //TODO
    }
}
