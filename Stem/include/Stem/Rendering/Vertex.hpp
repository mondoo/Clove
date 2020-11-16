#pragma once

#include <Clove/Graphics/PipelineObject.hpp>

namespace garlic::inline stem {
    struct Vertex {
        clv::mth::vec3f position{ 0.0f, 0.0f, 0.0f };
        clv::mth::vec3f normal{ 0.0f, 0.0f, 0.0f };
        clv::mth::vec2f texCoord{ 0.0f, 0.0f };
        clv::mth::vec3f colour{ 0.0f, 0.0f, 0.0f };
        clv::mth::vec4i jointIds{ 0, 0, 0, 0 };
        clv::mth::vec4f weights{ 0.0f, 0.0f, 0.0f, 0.0f };

        static clv::gfx::VertexInputBindingDescriptor getInputBindingDescriptor() {
            return clv::gfx::VertexInputBindingDescriptor{
                .stride = sizeof(Vertex),
            };
        }
    };
}