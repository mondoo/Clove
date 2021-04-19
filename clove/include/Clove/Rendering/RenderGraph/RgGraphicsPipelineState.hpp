#pragma once

#include "Clove/Rendering/RenderGraph/RgId.hpp"
#include "Clove/Rendering/RenderGraph/RgImage.hpp"
#include "Clove/Rendering/RenderGraph/RgShader.hpp"

#include <Clove/Graphics/GhaRenderPass.hpp>

namespace garlic::clove {
    class RgGraphicsPipelineState {
        //TYPES
    public:
        struct RenderTargetBinding {//TODO: RenderTargetDescriptor?
            LoadOperation loadOp{};
            StoreOperation storeOp{};
            RgImage target{};
        };

        struct DepthStencilBinding {
            LoadOperation loadOp{};
            StoreOperation storeOp{};
            RgImage target{};
        };

        struct Descriptor {
            RgShader vertexShader;
            RgShader pixelShader;

            //NOTE: Will create the vertex input based off of clove's uber vertex

            //TODO: Viewport / scissor
            //TODO: raster state
            //TODO: depth state
            //TODO: Blending

            //NOTE: Will be used to create the render pass.
            std::vector<RenderTargetBinding> renderTargets{};
            DepthStencilBinding depthStencil{};
        };

        //VARIABLES
    private:
        ResourceIdType id{ INVALID_ID };

        //FUNCTIONS
    public:
        operator ResourceIdType() {
            return id;
        }
    };
}