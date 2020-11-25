#pragma once

#include <Clove/Graphics/PipelineObject.hpp>
#include <Clove/Maths/Vector.hpp>

namespace garlic::clove {
    struct Vertex {
        vec3f position{ 0.0f, 0.0f, 0.0f };
        vec3f normal{ 0.0f, 0.0f, 0.0f };
        vec2f texCoord{ 0.0f, 0.0f };
        vec3f colour{ 0.0f, 0.0f, 0.0f };
        vec4i jointIds{ 0, 0, 0, 0 };
        vec4f weights{ 0.0f, 0.0f, 0.0f, 0.0f };

        static VertexInputBindingDescriptor getInputBindingDescriptor() {
            return VertexInputBindingDescriptor{
                .stride = sizeof(Vertex),
            };
        }
    };
}