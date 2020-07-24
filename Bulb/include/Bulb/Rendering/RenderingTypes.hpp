#pragma once

//TODO: Remove
#include <Clove/Graphics/Vulkan/VulkanTypes.hpp>

namespace blb::rnd {
    struct Vertex {
        clv::mth::vec3f position;
        clv::mth::vec3f colour;
        clv::mth::vec2f texCoord;

        static clv::gfx::VertexInputBindingDescriptor getInputBindingDescriptor() {
            clv::gfx::VertexInputBindingDescriptor descriptor{};
            descriptor.binding = 0;
            descriptor.stride  = sizeof(Vertex);

            return descriptor;
        }

        //TODO: This should be done per vertex shader
        static std::vector<clv::gfx::VertexAttributeDescriptor> getVertexAttributes() {
            std::vector<clv::gfx::VertexAttributeDescriptor> attributes(3);

            attributes[0].binding  = 0;
            attributes[0].location = 0;
            attributes[0].format   = clv::gfx::VertexAttributeFormat::R32G32B32_SFLOAT;
            attributes[0].offset   = offsetof(Vertex, position);

            attributes[1].binding  = 0;
            attributes[1].location = 1;
            attributes[1].format   = clv::gfx::VertexAttributeFormat::R32G32B32_SFLOAT;
            attributes[1].offset   = offsetof(Vertex, colour);

            attributes[2].binding  = 0;
            attributes[2].location = 2;
            attributes[2].format   = clv::gfx::VertexAttributeFormat::R32G32_SFLOAT;
            attributes[2].offset   = offsetof(Vertex, texCoord);

            return attributes;
        }
    };
}