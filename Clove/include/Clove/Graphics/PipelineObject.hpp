#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx {
    class Shader;
    class RenderPass;
    class DescriptorSetLayout;

    enum class ElementState {
        Static,
        Dynamic
    };

    struct ViewportDescriptor {
        ElementState state{ ElementState::Static };
        mth::vec2i position{ 0.0f, 0.0f };
        mth::vec2ui size{ 0.0f, 0.0f };
    };

    struct ScissorDescriptor {
        ElementState state{ ElementState::Static };
        mth::vec2i position{ 0.0f, 0.0f };
        mth::vec2ui size{ 0.0f, 0.0f };
    };

    struct RasteriserDescriptor {
        //TODO
    };
}

namespace clv::gfx {
    /**
     * @brief Represents the state of the current graphics pipeline.
     */
    class PipelineObject {
        //TYPES
    public:
        struct Descriptor {
            std::shared_ptr<Shader> vertexShader;
            std::shared_ptr<Shader> fragmentShader;

            VertexInputBindingDescriptor vertexInput;
            std::vector<VertexAttributeDescriptor> vertexAttributes;

            ViewportDescriptor viewportDescriptor;
            ScissorDescriptor scissorDescriptor;

            RasteriserDescriptor rasteriserDescriptor;

            std::shared_ptr<RenderPass> renderPass;

            std::vector<std::shared_ptr<DescriptorSetLayout>> descriptorSetLayouts;
        };

        //FUNCTIONS
    public:
        virtual ~PipelineObject() = default;
    };
}