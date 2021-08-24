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

    struct RgImageBindng {
        uint32_t slot{};
        RgResourceIdType image{};
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
    };

    struct RgDepthStencilBinding {
        LoadOperation loadOp{};
        StoreOperation storeOp{};
        DepthStencilValue clearValue{};
        RgResourceIdType target{};
    };
}