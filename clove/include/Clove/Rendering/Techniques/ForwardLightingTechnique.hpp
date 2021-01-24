#pragma once

#include "Clove/Rendering/Techniques/Technique.hpp"
#include "Clove/Rendering/RenderPasses/ForwardColourPass.hpp"

namespace garlic::clove {
    using ForwardLightingTechnique = Technique<ForwardColourPass>;
}