#pragma once
//
#include <Clove/Graphics/GhaRenderPass.hpp>
//

#include "Clove/Rendering/RenderGraph/RgBuffer.hpp"
#include "Clove/Rendering/RenderGraph/RgImage.hpp"

#include <Clove/Graphics/GhaGraphicsPipelineObject.hpp>
#include <Clove/Graphics/GhaSampler.hpp>
#include <Clove/Graphics/GhaShader.hpp>
#include <memory>
#include <vector>

namespace garlic::clove {
    //TODO: Desc. Used to create pipeline / renderpass?
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
            std::shared_ptr<GhaShader> vertexShader;
            std::shared_ptr<GhaShader> pixelShader;

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
        GhaGraphicsPipelineObject::Descriptor ghaDescriptor{};

        //TODO: Keep some sort of internal ID for tracking

        //FUNCTIONS
    public:
        RgGraphicsPipelineState();
        RgGraphicsPipelineState(Descriptor descriptor);
        
        //TODO:
    };
}