#pragma once

#include "Clove/Rendering/RenderTarget.hpp"

#include <Clove/Delegate/DelegateHandle.hpp>

namespace garlic::clove {
    class Swapchain;
    class PresentQueue;
    class GraphicsQueue;
    class GraphicsDevice;
    class GraphicsFactory;
    class Fence;
}

namespace clv::plt {
    class Window;
}

namespace garlic::clove {
    /**
     * @brief Swapchain backed RenderTarget.
     */
    class SwapchainRenderTarget : public RenderTarget {
        //VARIABLES
    private:
        garlic::clove::GraphicsDevice *graphicsDevice;
        std::shared_ptr<garlic::clove::GraphicsFactory> graphicsFactory;

        std::shared_ptr<garlic::clove::Swapchain> swapchain;
        std::shared_ptr<garlic::clove::PresentQueue> presentQueue;
        std::shared_ptr<garlic::clove::GraphicsQueue> graphicsQueue;

        std::vector<std::shared_ptr<garlic::clove::Semaphore>> renderFinishedSemaphores;
        std::vector<std::shared_ptr<garlic::clove::Semaphore>> imageAvailableSemaphores;
        std::vector<std::shared_ptr<garlic::clove::Fence>> framesInFlight;
        std::vector<std::shared_ptr<garlic::clove::Fence>> imagesInFlight;

        vec2ui windowSize{};
        DelegateHandle windowResizeHandle;

        bool requiresNewSwapchain{ false };

        //FUNCTIONS
    public:
        SwapchainRenderTarget();

        SwapchainRenderTarget(SwapchainRenderTarget const &other);
        SwapchainRenderTarget(SwapchainRenderTarget &&other) noexcept;

        SwapchainRenderTarget &operator=(SwapchainRenderTarget const &other);
        SwapchainRenderTarget &operator=(SwapchainRenderTarget &&other) noexcept;

        ~SwapchainRenderTarget();

        Expected<uint32_t, std::string> aquireNextImage(size_t const frameId) override;

        void submit(uint32_t imageIndex, size_t const frameId, garlic::clove::GraphicsSubmitInfo submission) override;

        garlic::clove::GraphicsImage::Format getImageFormat() const override;
        vec2ui getSize() const override;

        std::vector<std::shared_ptr<garlic::clove::GraphicsImageView>> getImageViews() const override;

    private:
        void onWindowSizeChanged(vec2ui const &size);
        void createSwapchain();
    };
}