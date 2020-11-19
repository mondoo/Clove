#include "Stem/Rendering/SwapchainRenderTarget.hpp"

#include "Stem/Application.hpp"

#include <Clove/Graphics/GraphicsDevice.hpp>
#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/PresentQueue.hpp>
#include <Clove/Graphics/Semaphore.hpp>
#include <Clove/Graphics/Swapchain.hpp>
#include <Clove/Platform/Window.hpp>
#include <Root/Log/Log.hpp>

namespace garlic::inline stem {
    SwapchainRenderTarget::SwapchainRenderTarget() {
        auto const graphicsFactory{ Application::get().getGraphicsDevice()->getGraphicsFactory() };
        auto const window{ Application::get().getWindow() };

        windowSize         = window->getSize();
        windowResizeHandle = window->onWindowResize.bind(&SwapchainRenderTarget::onWindowSizeChanged, this);

        auto expectedPresentQueue{ graphicsFactory->createPresentQueue() };
        if(expectedPresentQueue.hasValue()) {
            presentQueue = std::move(expectedPresentQueue.getValue());
        } else {
            GARLIC_ASSERT(false, expectedPresentQueue.getError());
        }

        createSwapchain();
    }

    SwapchainRenderTarget::SwapchainRenderTarget(SwapchainRenderTarget &&other) noexcept = default;

    SwapchainRenderTarget &SwapchainRenderTarget::operator=(SwapchainRenderTarget &&other) noexcept = default;

    SwapchainRenderTarget::~SwapchainRenderTarget() = default;

    Expected<uint32_t, std::string> SwapchainRenderTarget::aquireNextImage(std::shared_ptr<clv::gfx::Semaphore> availableSemaphore) {
        if(windowSize.x == 0 || windowSize.y == 0) {
            return Unexpected<std::string>{ "Cannot acquire image while Window is minimised." };
        }

        auto [imageIndex, result] = swapchain->aquireNextImage(availableSemaphore.get());
        if(result == clv::gfx::Result::Error_SwapchainOutOfDate) {
            createSwapchain();
            return Unexpected<std::string>{ "Swapchain was recreated." };
        }

        return imageIndex;
    }

    void SwapchainRenderTarget::present(uint32_t imageIndex, std::vector<std::shared_ptr<clv::gfx::Semaphore>> waitSemaphores) {
        auto result = presentQueue->present(clv::gfx::PresentInfo{
            .waitSemaphores = waitSemaphores,
            .swapChain      = swapchain,
            .imageIndex     = imageIndex,
        });

        if(result == clv::gfx::Result::Error_SwapchainOutOfDate || result == clv::gfx::Result::Success_SwapchainSuboptimal) {
            createSwapchain();
        }
    }

    clv::gfx::GraphicsImage::Format SwapchainRenderTarget::getImageFormat() const {
        return swapchain->getImageFormat();
    }

    clv::mth::vec2ui SwapchainRenderTarget::getSize() const {
        return swapchain->getSize();
    }

    std::vector<std::shared_ptr<clv::gfx::GraphicsImageView>> SwapchainRenderTarget::getImageViews() const {
        return swapchain->getImageViews();
    }

    void SwapchainRenderTarget::onWindowSizeChanged(clv::mth::vec2ui const &size) {
        windowSize = size;
        createSwapchain();
    }

    void SwapchainRenderTarget::createSwapchain() {
        if(windowSize.x == 0 || windowSize.y == 0) {
            return;
        }

        auto const graphicsDevice  = Application::get().getGraphicsDevice();
        auto const graphicsFactory = graphicsDevice->getGraphicsFactory();

        graphicsDevice->waitForIdleDevice();

        auto expectedSwapchain{ graphicsFactory->createSwapChain({ windowSize }) };
        if(expectedSwapchain.hasValue()) {
            swapchain = std::move(expectedSwapchain.getValue());
        } else {
            GARLIC_ASSERT(false, expectedSwapchain.getError());
        }

        onPropertiesChanged.broadcast();
    }
}