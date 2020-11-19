#pragma once

#include "Stem/Rendering/RenderTarget.hpp"

#include <Root/Delegate/DelegateHandle.hpp>

namespace clv::gfx {
    class Swapchain;
    class PresentQueue;
}

namespace clv::plt{
    class Window;
}

namespace garlic::inline stem {
    /**
     * @brief Swapchain backed RenderTarget.
     */
    class SwapchainRenderTarget : public RenderTarget {
        //VARIABLES
    private:
        std::shared_ptr<clv::gfx::Swapchain> swapchain;
        std::shared_ptr<clv::gfx::PresentQueue> presentQueue;

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

        Expected<uint32_t, std::string> aquireNextImage(std::shared_ptr<clv::gfx::Semaphore> availableSemaphore) override;

        void present(uint32_t imageIndex, std::vector<std::shared_ptr<clv::gfx::Semaphore>> waitSemaphores) override;

        clv::gfx::GraphicsImage::Format getImageFormat() const override;
        clv::mth::vec2ui getSize() const override;

        std::vector<std::shared_ptr<clv::gfx::GraphicsImageView>> getImageViews() const override;

    private:
        void onWindowSizeChanged(clv::mth::vec2ui const &size);
        void createSwapchain();
    };
}