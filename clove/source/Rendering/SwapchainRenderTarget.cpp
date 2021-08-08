#include "Clove/Rendering/SwapchainRenderTarget.hpp"

#include "Clove/Application.hpp"
#include "Clove/Surface.hpp"

#include <Clove/Graphics/GhaDevice.hpp>
#include <Clove/Graphics/GhaFactory.hpp>
#include <Clove/Graphics/GhaFence.hpp>
#include <Clove/Graphics/GhaPresentQueue.hpp>
#include <Clove/Graphics/GhaSemaphore.hpp>
#include <Clove/Graphics/GhaSwapchain.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    SwapchainRenderTarget::SwapchainRenderTarget(Surface &swapchainSurface, GhaDevice *graphicsDevice, uint32_t imageCount)
        : graphicsDevice{ graphicsDevice }
        , imageCount{ imageCount } {
        graphicsFactory = graphicsDevice->getGraphicsFactory();

        surfaceSize         = swapchainSurface.getSize();
        surfaceResizeHandle = swapchainSurface.onSurfaceResize().bind(&SwapchainRenderTarget::onSurfaceSizeChanged, this);

        presentQueue = *graphicsFactory->createPresentQueue();

        createSwapchain();
    }

    SwapchainRenderTarget::SwapchainRenderTarget(SwapchainRenderTarget &&other) noexcept = default;

    SwapchainRenderTarget &SwapchainRenderTarget::operator=(SwapchainRenderTarget &&other) noexcept = default;

    SwapchainRenderTarget::~SwapchainRenderTarget() = default;

    Expected<uint32_t, std::string> SwapchainRenderTarget::aquireNextImage(std::shared_ptr<GhaSemaphore> signalSemaphore) {
        if(surfaceSize.x == 0 || surfaceSize.y == 0) {
            return Unexpected<std::string>{ "Cannot acquire image while Window is minimised." };
        }

        if(requiresNewSwapchain) {
            createSwapchain();
            return Unexpected<std::string>{ "GhaSwapchain was recreated." };
        }

        auto const [imageIndex, result] = swapchain->aquireNextImage(signalSemaphore.get());
        if(result == Result::Error_SwapchainOutOfDate) {
            createSwapchain();
            return Unexpected<std::string>{ "GhaSwapchain was recreated." };
        }

        return imageIndex;
    }

    void SwapchainRenderTarget::present(uint32_t imageIndex, std::vector<std::shared_ptr<GhaSemaphore>> waitSemaphores) {
        Result const result = presentQueue->present(PresentInfo{
            .waitSemaphores = waitSemaphores,
            .swapChain      = swapchain,
            .imageIndex     = imageIndex,
        });

        if(result == Result::Error_SwapchainOutOfDate || result == Result::Success_SwapchainSuboptimal) {
            createSwapchain();
        }
    }

    GhaImage::Format SwapchainRenderTarget::getImageFormat() const {
        return swapchain->getImageFormat();
    }

    vec2ui SwapchainRenderTarget::getSize() const {
        return swapchain->getSize();
    }

    std::vector<std::shared_ptr<GhaImageView>> SwapchainRenderTarget::getImageViews() const {
        return swapchain->getImageViews();
    }

    void SwapchainRenderTarget::onSurfaceSizeChanged(vec2ui const &size) {
        surfaceSize          = size;
        requiresNewSwapchain = true;
    }

    void SwapchainRenderTarget::createSwapchain() {
        requiresNewSwapchain = true;

        if(surfaceSize.x == 0 || surfaceSize.y == 0) {
            return;
        }

        onPropertiesChangedBegin.broadcast();

        graphicsDevice->waitForIdleDevice();

        swapchain.reset();
        swapchain = *graphicsFactory->createSwapChain(GhaSwapchain::Descriptor{
            .extent     = surfaceSize,
            .imageCount = imageCount,
        });

        onPropertiesChangedEnd.broadcast();

        requiresNewSwapchain = false;
    }
}