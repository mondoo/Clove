#include "Clove/Rendering/RenderPasses/SkinningPass.hpp"

extern "C" const char constants[];
extern "C" const size_t constantsLength;

extern "C" const char skinning_c[];
extern "C" const size_t skinning_cLength;

namespace clove {
    SkinningPass::SkinningPass() = default;

    SkinningPass::SkinningPass(SkinningPass &&other) noexcept = default;

    SkinningPass &SkinningPass::operator=(SkinningPass &&other) noexcept = default;

    SkinningPass::~SkinningPass() = default;

    void SkinningPass::execute(RenderGraph &renderGraph, PassData const &passData) {
        //TODO
    }
}
