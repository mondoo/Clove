#include "Stem/Rendering/GraphicsImageRenderTarget.hpp"

#include "Stem/Application.hpp"

#include <Clove/Graphics/Fence.hpp>
#include <Clove/Graphics/GraphicsDevice.hpp>
#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/GraphicsQueue.hpp>

namespace garlic::inline stem {
    GraphicsImageRenderTarget::GraphicsImageRenderTarget(clv::gfx::GraphicsImage::Descriptor imageDescriptor)
        : imageDescriptor{ std::move(imageDescriptor) } {
        auto const graphicsFactory = Application::get().getGraphicsDevice()->getGraphicsFactory();

        //We won't be allocating any buffers from this queue, only using it to submit
        graphicsQueue = graphicsFactory->createGraphicsQueue(clv::gfx::CommandQueueDescriptor{ .flags = clv::gfx::QueueFlags::None });
        frameInFlight = graphicsFactory->createFence({ true });

        createImages();
    }

    GraphicsImageRenderTarget::GraphicsImageRenderTarget(GraphicsImageRenderTarget &&other) noexcept = default;

    GraphicsImageRenderTarget &GraphicsImageRenderTarget::operator=(GraphicsImageRenderTarget &&other) noexcept = default;

    GraphicsImageRenderTarget::~GraphicsImageRenderTarget() = default;

    Expected<uint32_t, std::string> GraphicsImageRenderTarget::aquireNextImage(size_t const frameId) {
        //Because we only have one frame, just wait for the graphics queue to finish using it then return
        frameInFlight->wait();
        frameInFlight->reset();

        return 0;
    }

    void GraphicsImageRenderTarget::submit(uint32_t imageIndex, size_t const frameId, clv::gfx::GraphicsSubmitInfo primarySubmission, std::vector<clv::gfx::GraphicsSubmitInfo> secondarySubmissions) {
        secondarySubmissions.emplace_back(std::move(primarySubmission));
        graphicsQueue->submit(secondarySubmissions, frameInFlight.get());
    }

    clv::gfx::GraphicsImage::Format GraphicsImageRenderTarget::getImageFormat() const {
        return imageDescriptor.format;
    }

    clv::mth::vec2ui GraphicsImageRenderTarget::getSize() const {
        return imageDescriptor.dimensions;
    }

    std::vector<std::shared_ptr<clv::gfx::GraphicsImageView>> GraphicsImageRenderTarget::getImageViews() const {
        return { renderTargetView };
    }

    void GraphicsImageRenderTarget::resize(clv::mth::vec2ui size) {
        imageDescriptor.dimensions = std::move(size);
        createImages();
    }

    std::shared_ptr<clv::gfx::GraphicsImage> GraphicsImageRenderTarget::getNextReadyImage() {
        //Stall until we are ready to return the image.
        frameInFlight->wait();
        return renderTargetImage;
    }

    void GraphicsImageRenderTarget::createImages() {
        auto const device{ Application::get().getGraphicsDevice() };
        auto const factory{ device->getGraphicsFactory() };

        //Make sure we're not using the image when we re-create it
        frameInFlight->wait();

        renderTargetImage = factory->createImage(imageDescriptor);
        renderTargetView  = renderTargetImage->createView(clv::gfx::GraphicsImageView::Descriptor{
            .type       = clv::gfx::GraphicsImageView::Type::_2D,
            .layer      = 0,
            .layerCount = 1,
        });

        onPropertiesChanged.broadcast();
    }
}