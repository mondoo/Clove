#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/Shader.hpp"

namespace clv::gfx {
    class Shader;
    class RenderPass;
    class DescriptorSetLayout;

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

    struct PushConstantDescriptor {
        Shader::Stage stage;
        uint32_t size{ 0 };
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