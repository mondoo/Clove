#include "Clove/Rendering/SwapchainRenderTarget.hpp"

#include "Clove/Application.hpp"
#include "Clove/Surface.hpp"

#include <Clove/Graphics/Fence.hpp>
#include <Clove/Graphics/GraphicsDevice.hpp>
#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/PresentQueue.hpp>
#include <Clove/Graphics/Semaphore.hpp>
#include <Clove/Graphics/Swapchain.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    SwapchainRenderTarget::SwapchainRenderTarget()
        : graphicsDevice{ Application::get().getGraphicsDevice() } {
        graphicsFactory = graphicsDevice->getGraphicsFactory();

        //Get the surface (window) that this render
        auto const surface{ Application::get().getSurface() };

        surfaceSize         = surface->getSize();
        surfaceResizeHandle = surface->onSurfaceResize().bind(&SwapchainRenderTarget::onSurfaceSizeChanged, this);

        presentQueue = *graphicsFactory->createPresentQueue();

        //We won't be allocating any buffers from this queue, only using it to submit
        graphicsQueue = *graphicsFactory->createGraphicsQueue(garlic::clove::CommandQueueDescriptor{ .flags = garlic::clove::QueueFlags::None });

        createSwapchain();
    }

    SwapchainRenderTarget::SwapchainRenderTarget(SwapchainRenderTarget &&other) noexcept = default;

    SwapchainRenderTarget &SwapchainRenderTarget::operator=(SwapchainRenderTarget &&other) noexcept = default;

    SwapchainRenderTarget::~SwapchainRenderTarget() = default;

    Expected<uint32_t, std::string> SwapchainRenderTarget::aquireNextImage(size_t const frameId) {
        if(surfaceSize.x == 0 || surfaceSize.y == 0) {
            return Unexpected<std::string>{ "Cannot acquire image while Window is minimised." };
        }

        if(requiresNewSwapchain) {
            createSwapchain();
            return Unexpected<std::string>{ "Swapchain was recreated." };
        }

        if(std::size(imageAvailableSemaphores) <= frameId) {
            imageAvailableSemaphores.emplace_back(*graphicsFactory->createSemaphore());
        }
        if(std::size(framesInFlight) <= frameId) {
            framesInFlight.emplace_back(*graphicsFactory->createFence({ true }));
        }

        //Wait for the graphics queue to be finished with the frame we want to render
        framesInFlight[frameId]->wait();

        auto const [imageIndex, result] = swapchain->aquireNextImage(imageAvailableSemaphores[frameId].get());
        if(result == garlic::clove::Result::Error_SwapchainOutOfDate) {
            createSwapchain();
            return Unexpected<std::string>{ "Swapchain was recreated." };
        }

        //Make sure we're not about to start using an image that hasn't been rendered to yet
        if(imagesInFlight[imageIndex] != nullptr) {
            imagesInFlight[imageIndex]->wait();
        }
        imagesInFlight[imageIndex] = framesInFlight[frameId];

        framesInFlight[frameId]->reset();

        return imageIndex;
    }

    void SwapchainRenderTarget::submit(uint32_t imageIndex, size_t const frameId, garlic::clove::GraphicsSubmitInfo submission) {
        if(std::size(renderFinishedSemaphores) <= frameId) {
            renderFinishedSemaphores.emplace_back(*graphicsFactory->createSemaphore());
        }

        //Inject the sempahores we use to synchronise with the swapchain and present queue
        submission.waitSemaphores.push_back({ imageAvailableSemaphores[frameId], garlic::clove::PipelineObject::Stage::ColourAttachmentOutput });
        submission.signalSemaphores.push_back(renderFinishedSemaphores[frameId]);

        graphicsQueue->submit({ std::move(submission) }, framesInFlight[frameId].get());

        auto const result = presentQueue->present(garlic::clove::PresentInfo{
            .waitSemaphores = { renderFinishedSemaphores[frameId] },
            .swapChain      = swapchain,
            .imageIndex     = imageIndex,
        });

        if(result == garlic::clove::Result::Error_SwapchainOutOfDate || result == garlic::clove::Result::Success_SwapchainSuboptimal) {
            createSwapchain();
        }
    }

    garlic::clove::GraphicsImage::Format SwapchainRenderTarget::getImageFormat() const {
        return swapchain->getImageFormat();
    }

    vec2ui SwapchainRenderTarget::getSize() const {
        return swapchain->getSize();
    }

    std::vector<std::shared_ptr<garlic::clove::GraphicsImageView>> SwapchainRenderTarget::getImageViews() const {
        return swapchain->getImageViews();
    }

    void SwapchainRenderTarget::onSurfaceSizeChanged(vec2ui const &size) {
        surfaceSize           = size;
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
        swapchain = *graphicsFactory->createSwapChain({ surfaceSize });

        imagesInFlight.resize(std::size(swapchain->getImageViews()));

        onPropertiesChangedEnd.broadcast();

        requiresNewSwapchain = false;
    }
}