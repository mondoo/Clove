#pragma once

#include "Clove/Rendering/RenderGraph/RgBuffer.hpp"
#include "Clove/Rendering/RenderGraph/RgImage.hpp"

#include <Clove/Graphics/GhaGraphicsPipelineObject.hpp>
#include <Clove/Graphics/GhaRenderPass.hpp>
#include <Clove/Graphics/GhaShader.hpp>
#include <Clove/Graphics/GhaSampler.hpp>
#include <memory>
#include <optional>
#include <vector>

namespace garlic::clove {
    //TODO: Desc. Used to create pipeline / renderpass?
    class RgGraphicsPipelineState {
        //TYPES
    public:
        struct BufferBinding {
            uint32_t slot{};
            RgBuffer buffer{};
        };

        struct ImageBindng {
            uint32_t slot{};
            GhaSampler::Descriptor samplerState{};//TODO: RG version?
            RgImage image{};
        };

        struct RenderTargetBinding {//TODO: RenderTargetDescriptor?
            LoadOperation loadOp{};
            StoreOperation storeOp{};
            std::optional<RgImage> target{};//TODO: If set will render to that else will render to screen? Or have an inbuilt scene colour RgImage
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

            //NOTE: Will be used to create descriptor sets
            std::vector<BufferBinding> shaderUbos{};
            std::vector<ImageBindng> shaderCombinedImageSamplers{};

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
        //TODO:
    };
}