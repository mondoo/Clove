#pragma once

#include "Clove/Rendering/RenderGraph/RgPass.hpp"

#include <vector>

namespace garlic::clove {
    struct BufferBinding {
        uint32_t slot{};
        ResourceIdType buffer{};
    };

    struct ImageBindng {
        uint32_t slot{};
        GhaSampler::Descriptor samplerState{};//TODO: RG version?
        ResourceIdType image{};
    };

    struct RenderTargetBinding {
        LoadOperation loadOp{};
        StoreOperation storeOp{};
        ColourValue clearColour{};
        ResourceIdType target{};
    };

    struct DepthStencilBinding {
        LoadOperation loadOp{};
        StoreOperation storeOp{};
        DepthStencilValue clearValue{};
        ResourceIdType target{};
    };
}

namespace garlic::clove {
    class RgRenderPass : public RgPass {
        //TYPES
    public:
        struct Descriptor {
            RgShader vertexShader{};
            RgShader pixelShader{};

            vec2i viewportPosition{ 0 };
            vec2ui viewportSize{ 0 };
            //TODO: raster state
            bool depthTest{ true };
            bool depthWrite{ true };
            bool enableBlending{ true };

            std::vector<RenderTargetBinding> renderTargets{};
            DepthStencilBinding depthStencil{};
        };

        /**
         * @brief Represents a unit of work, single draw indexed call.
         */
        struct Submission {
            ResourceIdType vertexBuffer{};
            ResourceIdType indexBuffer{};

            std::vector<BufferBinding> shaderUbos{};
            std::vector<ImageBindng> shaderCombinedImageSamplers{};

            size_t indexCount{ 0 };
        };

        //VARIABLES
    private:
        
        //FUNCTIONS
    public:
        RgRenderPass() = delete;
        RgRenderPass(PassIdType id);

        RgRenderPass(RgRenderPass const &other) = delete;
        RgRenderPass(RgRenderPass &&other) noexcept;

        RgRenderPass &operator=(RgRenderPass const &other) = delete;
        RgRenderPass &operator=(RgRenderPass &&other) noexcept;

        ~RgRenderPass();
    };
}