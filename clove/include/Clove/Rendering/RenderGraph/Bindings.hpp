#pragma once

#include "Clove/Rendering/RenderGraph/RgId.hpp"

#include <Clove/Graphics/GhaGraphicsCommandBuffer.hpp>
#include <Clove/Graphics/GhaRenderPass.hpp>
#include <Clove/Graphics/GhaSampler.hpp>
#include <Clove/Graphics/GhaShader.hpp>

namespace clove {
    struct RgBufferBinding {
        uint32_t slot{};
        RgResourceIdType buffer{};
        GhaShader::Stage shaderStage{};//TEMP: Manually set shader stage in place of shader reflection.
    };

    struct RgImageBinding {
        uint32_t slot{};
        RgResourceIdType image{};
        uint32_t arrayIndex{ 0 }; /**< If image is an image array, which index to bind from.*/
        uint32_t arrayCount{ 1 }; /**< If image is an image array, how many images to bind. */
    };

    struct RgSamplerBinding {
        uint32_t slot{};
        RgResourceIdType sampler{};
    };

    struct RgRenderTargetBinding {
        LoadOperation loadOp{};
        StoreOperation storeOp{};
        ColourValue clearColour{};
        RgResourceIdType target{};
        uint32_t imageArrayIndex{ 0 }; /**< If image is an image array, which index to bind from. */
        uint32_t imageArrayCount{ 1 }; /**< If image is an image array, how many images to bind. */
    };

    struct RgDepthStencilBinding {
        LoadOperation loadOp{};
        StoreOperation storeOp{};
        DepthStencilValue clearValue{};
        RgResourceIdType target{};
        uint32_t imageArrayIndex{ 0 }; /**< If image is an image array, which index to bind from.*/
        uint32_t imageArrayCount{ 1 }; /**< If image is an image array, how many images to bind. */
    };
}