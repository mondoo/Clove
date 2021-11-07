#pragma once

#include <Clove/Maths/Vector.hpp>

namespace clove {
    struct PointLightComponent {
        vec3f ambientColour{ 0.01f, 0.01f, 0.01f };
        vec3f diffuseColour{ 0.75f, 0.75f, 0.75f };
        vec3f specularColour{ 1.0f, 1.0f, 1.0f };
    };
}