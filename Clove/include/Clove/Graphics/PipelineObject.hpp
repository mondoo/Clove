#pragma once

#include "Clove/Graphics/Shader.hpp"

namespace clv::gfx {
    class Shader;
    class RenderPass;
    class DescriptorSetLayout;

    enum class VertexAttributeFormat {
        R32_SFLOAT,
        R32G32_SFLOAT,
        R32G32B32_SFLOAT,
        R32G32B32A32_SFLOAT,
        R32G32B32A32_SINT,
    };

    struct VertexInputBindingDescriptor {
        uint32_t stride{ 0 }; /**< Size of each vertex buffer element. */
    };

    struct VertexAttributeDescriptor {
        uint32_t location{ 0 }; /**< Maps to the location specified in the shader. */
        VertexAttributeFormat format;
        uint32_t offset{ 0 }; /**< Offset into the vertex buffer element of this attribute. */
    };

    enum class ElementState {
        Static,
        Dynamic
    };

    struct AreaDescriptor {
        ElementState state{ ElementState::Static };
        mth::vec2i position{ 0.0f, 0.0f };
        mth::vec2ui size{ 0.0f, 0.0f };
    };

    struct RasteriserDescriptor {
        //TODO
    };

    struct DepthStateDescriptor{
        bool depthTest{ true };
        bool depthWrite{ true };
    };

    struct PushConstantDescriptor {
        Shader::Stage stage;
        size_t offset{ 0 }; /**< Offset specified in the shader (layout(offset = x)). Required if using different push constants for different stages. */
        size_t size{ 0 };
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

            AreaDescriptor viewportDescriptor;
            AreaDescriptor scissorDescriptor;

            RasteriserDescriptor rasteriserDescriptor;

            DepthStateDescriptor depthState;

            std::shared_ptr<RenderPass> renderPass;

            std::vector<std::shared_ptr<DescriptorSetLayout>> descriptorSetLayouts;
            std::vector<PushConstantDescriptor> pushConstants;
        };

        enum class Stage {
            Top,
            Transfer,
            PixelShader,
            EarlyPixelTest,
            ColourAttachmentOutput,
        };

        //FUNCTIONS
    public:
        virtual ~PipelineObject() = default;
    };
}