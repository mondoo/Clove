#include "Stem/Rendering/GraphicsImageRenderTarget.hpp"

#include "Stem/Application.hpp"

#include <Clove/Graphics/GraphicsDevice.hpp>
#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/Semaphore.hpp>

namespace garlic::inline stem {
    GraphicsImageRenderTarget::GraphicsImageRenderTarget(clv::gfx::GraphicsImage::Descriptor imageDescriptor)
        : imageDescriptor{ std::move(imageDescriptor) }{
        createImages();
    }

    GraphicsImageRenderTarget::GraphicsImageRenderTarget(GraphicsImageRenderTarget &&other) noexcept = default;

    GraphicsImageRenderTarget &GraphicsImageRenderTarget::operator=(GraphicsImageRenderTarget &&other) noexcept = default;

    GraphicsImageRenderTarget::~GraphicsImageRenderTarget() = default;

    Expected<uint32_t, std::string> GraphicsImageRenderTarget::aquireNextImage(std::shared_ptr<clv::gfx::Semaphore> availableSemaphore) {
        //Signal the semaphore straight away as we only have one image.
        //availableSemaphore->signal();
        return 0;
    }

    void GraphicsImageRenderTarget::present(uint32_t imageIndex, std::vector<std::shared_ptr<clv::gfx::Semaphore>> waitSemaphores) {
        //Wait on the semaphores to be signaled as we've only got one image and are not doing any fancy double / tripple buffering.
        for(auto &semaphore : waitSemaphores) {
            //semaphore->wait();
        }
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

    std::shared_ptr<clv::gfx::GraphicsImage> GraphicsImageRenderTarget::getPresentedImage() {
        //Return the only image available as double / tripple buffering is not yet implemented.
        return renderTargetImage;
    }

    void GraphicsImageRenderTarget::createImages() {
        auto const device{ Application::get().getGraphicsDevice() };
        auto const factory{ device->getGraphicsFactory() };

        renderTargetImage = factory->createImage(imageDescriptor);
        renderTargetView  = renderTargetImage->createView(clv::gfx::GraphicsImageView::Descriptor{
            .type       = clv::gfx::GraphicsImageView::Type::_2D,
            .layer      = 0,
            .layerCount = 1,
        });

        onPropertiesChanged.broadcast();
    }
}