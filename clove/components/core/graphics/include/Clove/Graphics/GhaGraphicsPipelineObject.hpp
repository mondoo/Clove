#pragma once

#include "Clove/Graphics/GhaShader.hpp"

#include <Clove/Maths/Vector.hpp>
#include <memory>
#include <vector>

namespace garlic::clove {
    class GhaShader;
    class GhaRenderPass;
    class GhaDescriptorSetLayout;

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
        ElementState state{ ElementState::Static }; /**< Dynamic states stop the pipeline object being recreated if changed but require their size is recorded everyframe. */
        vec2i position{ 0 };                        /**< Not required if state is dynamic. */
        vec2ui size{ 0 };                           /**< Not required if state is dynamic. */
    };

    struct RasteriserDescriptor {
        //TODO
    };

    struct DepthStateDescriptor {
        bool depthTest{ true };
        bool depthWrite{ true };
    };

    struct PushConstantDescriptor {
        GhaShader::Stage stage;
        size_t offset{ 0 }; /**< Offset specified in the shader (layout(offset = x)). Required if using different push constants for different stages. */
        size_t size{ 0 };
    };
}

namespace garlic::clove {
    /**
     * @brief Represents the state of the current graphics pipeline.
     */
    class GhaGraphicsPipelineObject {
        //TYPES
    public:
        struct Descriptor {
            std::shared_ptr<GhaShader> vertexShader;
            std::shared_ptr<GhaShader> fragmentShader;

            VertexInputBindingDescriptor vertexInput;
            std::vector<VertexAttributeDescriptor> vertexAttributes;

            AreaDescriptor viewportDescriptor;
            AreaDescriptor scissorDescriptor;

            RasteriserDescriptor rasteriserDescriptor;

            DepthStateDescriptor depthState;

            bool enableBlending{ true };

            std::shared_ptr<GhaRenderPass> renderPass;

            std::vector<std::shared_ptr<GhaDescriptorSetLayout>> descriptorSetLayouts;
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
        virtual ~GhaGraphicsPipelineObject() = default;
    };
}