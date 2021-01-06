#pragma once

#include "Clove/Rendering/RenderTarget.hpp"

#include <Clove/Delegate/DelegateHandle.hpp>
#include <vector>

namespace garlic::clove {
    class Swapchain;
    class PresentQueue;
    class GraphicsQueue;
    class GraphicsDevice;
    class GraphicsFactory;
    class Fence;
    class Surface;
}

namespace garlic::clove {
    /**
     * @brief Swapchain backed RenderTarget.
     */
    class SwapchainRenderTarget : public RenderTarget {
        //VARIABLES
    private:
        GraphicsDevice *graphicsDevice;
        std::shared_ptr<GraphicsFactory> graphicsFactory;

        std::shared_ptr<Swapchain> swapchain;
        std::shared_ptr<PresentQueue> presentQueue;
        std::shared_ptr<GraphicsQueue> graphicsQueue;

        std::vector<std::shared_ptr<Semaphore>> renderFinishedSemaphores;
        std::vector<std::shared_ptr<Semaphore>> imageAvailableSemaphores;
        std::vector<std::shared_ptr<Fence>> framesInFlight;
        std::vector<std::shared_ptr<Fence>> imagesInFlight;

        vec2ui surfaceSize{};
        DelegateHandle surfaceResizeHandle;

        bool requiresNewSwapchain{ false };

        //FUNCTIONS
    public:
        SwapchainRenderTarget() = delete;
        SwapchainRenderTarget(Surface &swapchainSurface, GraphicsDevice *graphicsDevice);

        SwapchainRenderTarget(SwapchainRenderTarget const &other);
        SwapchainRenderTarget(SwapchainRenderTarget &&other) noexcept;

        SwapchainRenderTarget &operator=(SwapchainRenderTarget const &other);
        SwapchainRenderTarget &operator=(SwapchainRenderTarget &&other) noexcept;

        ~SwapchainRenderTarget();

        Expected<uint32_t, std::string> aquireNextImage(size_t const frameId) override;

        void submit(uint32_t imageIndex, size_t const frameId, GraphicsSubmitInfo submission) override;

        GraphicsImage::Format getImageFormat() const override;
        vec2ui getSize() const override;

        std::vector<std::shared_ptr<GraphicsImageView>> getImageViews() const override;

    private:
        void onSurfaceSizeChanged(vec2ui const &size);
        void createSwapchain();
    };
}