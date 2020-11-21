#pragma once

#include "Stem/Rendering/RenderTarget.hpp"

#include <Root/Delegate/DelegateHandle.hpp>

namespace clv::gfx {
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

namespace garlic::inline stem {
    /**
     * @brief Swapchain backed RenderTarget.
     */
    class SwapchainRenderTarget : public RenderTarget {
        //VARIABLES
    private:
        clv::gfx::GraphicsDevice *graphicsDevice;
        std::shared_ptr<clv::gfx::GraphicsFactory> graphicsFactory;

        std::shared_ptr<clv::gfx::Swapchain> swapchain;
        std::shared_ptr<clv::gfx::PresentQueue> presentQueue;
        std::shared_ptr<clv::gfx::GraphicsQueue> graphicsQueue;

        std::vector<std::shared_ptr<clv::gfx::Semaphore>> renderFinishedSemaphores;
        std::vector<std::shared_ptr<clv::gfx::Semaphore>> imageAvailableSemaphores;
        std::vector<std::shared_ptr<clv::gfx::Fence>> framesInFlight;
        std::vector<std::shared_ptr<clv::gfx::Fence>> imagesInFlight;

        clv::mth::vec2ui windowSize{};
        DelegateHandle windowResizeHandle;

        //FUNCTIONS
    public:
        SwapchainRenderTarget();

        SwapchainRenderTarget(SwapchainRenderTarget const &other);
        SwapchainRenderTarget(SwapchainRenderTarget &&other) noexcept;

        SwapchainRenderTarget &operator=(SwapchainRenderTarget const &other);
        SwapchainRenderTarget &operator=(SwapchainRenderTarget &&other) noexcept;

        ~SwapchainRenderTarget();

        Expected<uint32_t, std::string> aquireNextImage(size_t const frameId) override;

        void submit(uint32_t imageIndex, size_t const frameId, clv::gfx::GraphicsSubmitInfo primarySubmission, std::vector<clv::gfx::GraphicsSubmitInfo> secondarySubmissions) override;

        clv::gfx::GraphicsImage::Format getImageFormat() const override;
        clv::mth::vec2ui getSize() const override;

        std::vector<std::shared_ptr<clv::gfx::GraphicsImageView>> getImageViews() const override;

    private:
        void onWindowSizeChanged(clv::mth::vec2ui const &size);
        void createSwapchain();
    };
}