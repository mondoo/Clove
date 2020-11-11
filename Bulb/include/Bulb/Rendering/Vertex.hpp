#pragma once

#include <Clove/Graphics/PipelineObject.hpp>

namespace blb::rnd {
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

        //TODO: This should be done per vertex shader
        static std::vector<clv::gfx::VertexAttributeDescriptor> getVertexAttributes() {
            std::vector<clv::gfx::VertexAttributeDescriptor> attributes(6);

            attributes[0].location = 0;
            attributes[0].format   = clv::gfx::VertexAttributeFormat::R32G32B32_SFLOAT;
            attributes[0].offset   = offsetof(Vertex, position);

            attributes[1].location = 1;
            attributes[1].format   = clv::gfx::VertexAttributeFormat::R32G32B32_SFLOAT;
            attributes[1].offset   = offsetof(Vertex, normal);

            attributes[2].location = 2;
            attributes[2].format   = clv::gfx::VertexAttributeFormat::R32G32_SFLOAT;
            attributes[2].offset   = offsetof(Vertex, texCoord);

            attributes[3].location = 3;
            attributes[3].format   = clv::gfx::VertexAttributeFormat::R32G32B32_SFLOAT;
            attributes[3].offset   = offsetof(Vertex, colour);

            attributes[4].location = 4;
            attributes[4].format   = clv::gfx::VertexAttributeFormat::R32G32B32A32_SINT;
            attributes[4].offset   = offsetof(Vertex, jointIds);

            attributes[5].location = 5;
            attributes[5].format   = clv::gfx::VertexAttributeFormat::R32G32B32A32_SFLOAT;
            attributes[5].offset   = offsetof(Vertex, weights);

            return attributes;
        }
    };
}