#pragma once

#include "Clove/Rendering/RenderTarget.hpp"

#include <Clove/Delegate/DelegateHandle.hpp>
#include <vector>

namespace clove {
    class GhaSwapchain;
    class GhaPresentQueue;
    class GhaGraphicsQueue;
    class GhaDevice;
    class GhaFactory;
    class GhaFence;
    class Window;
}

namespace clove {
    /**
     * @brief GhaSwapchain backed RenderTarget.
     */
    class SwapchainRenderTarget : public RenderTarget {
        //VARIABLES
    private:
        GhaDevice *graphicsDevice{ nullptr };
        GhaFactory *graphicsFactory{ nullptr };

        uint32_t imageCount{};
        std::unique_ptr<GhaSwapchain> swapchain;
        std::unique_ptr<GhaPresentQueue> presentQueue;

        vec2ui windowSize{};
        DelegateHandle windowResizeHandle;

        bool requiresNewSwapchain{ false };

        //FUNCTIONS
    public:
        SwapchainRenderTarget() = delete;
        SwapchainRenderTarget(Window &swapchainWindow, GhaDevice *graphicsDevice, uint32_t imageCount);

        SwapchainRenderTarget(SwapchainRenderTarget const &other);
        SwapchainRenderTarget(SwapchainRenderTarget &&other) noexcept;

        SwapchainRenderTarget &operator=(SwapchainRenderTarget const &other);
        SwapchainRenderTarget &operator=(SwapchainRenderTarget &&other) noexcept;

        ~SwapchainRenderTarget();

        Expected<uint32_t, std::string> aquireNextImage(GhaSemaphore const *const signalSemaphore) override;

        void present(uint32_t imageIndex, std::vector<GhaSemaphore const *> waitSemaphores) override;

        GhaImage::Format getImageFormat() const override;
        vec2ui getSize() const override;

        std::vector<GhaImage *> getImages() const override;

    private:
        void onSurfaceSizeChanged(vec2ui const &size);
        void createSwapchain();
    };
}