#include "Clove/Components/TransformComponent.hpp"

#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>
#include <Clove/Maths/Maths.hpp>
#include <Clove/Maths/MathsHelpers.hpp>

namespace garlic::clove {
    vec3f TransformComponent::getForward() const {
        vec3f const eulerRot{ quaternionToEuler(rotation) };

        vec3f front;
        front.x = sin(eulerRot.y) * cos(eulerRot.x);
        front.y = sin(eulerRot.x);
        front.z = cos(eulerRot.y) * cos(eulerRot.x);

        return normalise(front);
    }

    vec3f TransformComponent::getRight() const {
        vec3f constexpr worldUp{ 0.0f, 1.0f, 0.0f };
        return normalise(cross(getForward(), worldUp));
    }

    vec3f TransformComponent::getUp() const {
        return normalise(cross(getRight(), getForward()));
    }
}