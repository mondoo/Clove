#pragma once

#include "Clove/Rendering/RenderGraph/RgId.hpp"

#include <Clove/Graphics/GhaGraphicsCommandBuffer.hpp>
#include <Clove/Graphics/GhaRenderPass.hpp>
#include <Clove/Graphics/GhaSampler.hpp>
#include <Clove/Graphics/GhaShader.hpp>

namespace clove {
    struct RgBufferBinding {
        uint32_t slot{};
        RgBufferId buffer{};
        size_t offset{}; /**< Offset into the buffer to start binding from. */
        size_t size{};   /**< Size of the binding within the buffer. */

        GhaShader::Stage shaderStage{};//TEMP: Manually set shader stage in place of shader reflection.
    };

    struct RgImageBinding {
        uint32_t slot{};
        RgImageId image{};
        uint32_t arrayIndex{ 0 };
        uint32_t arrayCount{ 1 };
    };

    struct RgSamplerBinding {
        uint32_t slot{};
        RgResourceId sampler{};
    };

    struct RgRenderTargetBinding {
        LoadOperation loadOp{};
        StoreOperation storeOp{};
        ClearValue clearValue{};
        RgImageId target{};
        uint32_t targetArrayIndex{ 0 };
        uint32_t targetArrayCount{ 1 };
    };

    struct RgDepthStencilBinding {
        LoadOperation loadOp{};
        StoreOperation storeOp{};
        DepthStencilValue clearValue{};
        RgImageId target{};
        uint32_t targetArrayIndex{ 0 };
        uint32_t targetArrayCount{ 1 };
    };
}