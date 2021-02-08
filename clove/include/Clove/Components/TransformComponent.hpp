#pragma once

#include <Clove/Maths/MathsTypes.hpp>
#include <Clove/Serialisation/Node.hpp>
#include <vector>

namespace garlic::clove {
    /**
     * @brief A TransformComponent holds the position, rotation and scale of an Entity.
     */
    struct TransformComponent {
        vec3f position{ 0.0f, 0.0f, 0.0f };
        quatf rotation{ vec3f{ 0.0f, 0.0f, 0.0f } };
        vec3f scale{ 1.0f, 1.0f, 1.0f };

        /**
         * @brief This matrix is calculated inside TransformLayer. One frame needs to pass
         * before it is updated
         */
        mat4f worldMatrix{ 1.0f };

        vec3f getForward() const;
        vec3f getRight() const;
        vec3f getUp() const;
    };
}

namespace garlic::clove {
    template<>
    inline serialiser::Node serialise(TransformComponent const &object) {
        serialiser::Node node{};
        node["position"] = object.position;
        node["rotation"] = object.rotation;
        node["scale"]    = object.scale;
        return node;
    }

    template<>
    inline TransformComponent deserialise(serialiser::Node const &node) {
        TransformComponent component{};
        component.position = node["position"].as<vec3f>();
        component.rotation = node["rotation"].as<quatf>();
        component.scale    = node["scale"].as<vec3f>();
        return component;
    }
}