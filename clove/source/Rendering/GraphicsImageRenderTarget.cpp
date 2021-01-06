#include "Clove/Rendering/GraphicsImageRenderTarget.hpp"

#include "Clove/Application.hpp"

#include <Clove/Graphics/Fence.hpp>
#include <Clove/Graphics/GraphicsBuffer.hpp>
#include <Clove/Graphics/GraphicsDevice.hpp>
#include <Clove/Graphics/GraphicsFactory.hpp>
#include <Clove/Graphics/GraphicsQueue.hpp>

namespace garlic::clove {
    GraphicsImageRenderTarget::GraphicsImageRenderTarget(GraphicsImage::Descriptor imageDescriptor, std::shared_ptr<GraphicsFactory> factory)
        : imageDescriptor{ std::move(imageDescriptor) }
        , factory{ std::move(factory) } {

        //We won't be allocating any buffers from this queue, only using it to submit
        graphicsQueue = *factory->createGraphicsQueue(CommandQueueDescriptor{ .flags = QueueFlags::None });
        transferQueue = *factory->createTransferQueue(CommandQueueDescriptor{ .flags = QueueFlags::ReuseBuffers });

        frameInFlight           = *factory->createFence({ true });
        renderFinishedSemaphore = *factory->createSemaphore();

        transferCommandBuffer = transferQueue->allocateCommandBuffer();

        createImages();
    }

    GraphicsImageRenderTarget::GraphicsImageRenderTarget(GraphicsImageRenderTarget &&other) noexcept = default;

    GraphicsImageRenderTarget &GraphicsImageRenderTarget::operator=(GraphicsImageRenderTarget &&other) noexcept = default;

    GraphicsImageRenderTarget::~GraphicsImageRenderTarget() = default;

    Expected<uint32_t, std::string> GraphicsImageRenderTarget::aquireNextImage(size_t const frameId) {
        //Because we only have one frame, just wait for the graphics/transfer queues to finish using it then return
        frameInFlight->wait();
        frameInFlight->reset();

        return 0;
    }

    void GraphicsImageRenderTarget::submit(uint32_t imageIndex, size_t const frameId, GraphicsSubmitInfo submission) {
        using namespace garlic::clove;

        submission.signalSemaphores.push_back(renderFinishedSemaphore);
        graphicsQueue->submit({ std::move(submission) }, nullptr);

        TransferSubmitInfo transferSubmission{
            .waitSemaphores = { { renderFinishedSemaphore, PipelineObject::Stage::Transfer } },
            .commandBuffers = { transferCommandBuffer },
        };
        transferQueue->submit({ std::move(transferSubmission) }, frameInFlight.get());
    }

    GraphicsImage::Format GraphicsImageRenderTarget::getImageFormat() const {
        return imageDescriptor.format;
    }

    vec2ui GraphicsImageRenderTarget::getSize() const {
        return imageDescriptor.dimensions;
    }

    std::vector<std::shared_ptr<GraphicsImageView>> GraphicsImageRenderTarget::getImageViews() const {
        return { renderTargetView };
    }

    void GraphicsImageRenderTarget::resize(vec2ui size) {
        imageDescriptor.dimensions = std::move(size);
        createImages();
    }

    std::shared_ptr<GraphicsBuffer> GraphicsImageRenderTarget::getNextReadyBuffer() {
        //Stall until we are ready to return the image.
        frameInFlight->wait();

        return renderTargetBuffer;
    }

    void GraphicsImageRenderTarget::createImages() {
        using namespace garlic::clove;

        //Make sure we're not using the image when we re-create it
        frameInFlight->wait();

        onPropertiesChangedBegin.broadcast();

        renderTargetImage = *factory->createImage(imageDescriptor);
        renderTargetView  = renderTargetImage->createView(GraphicsImageView::Descriptor{
            .type       = GraphicsImageView::Type::_2D,
            .layer      = 0,
            .layerCount = 1,
        });

        size_t constexpr bytesPerPixel{ 4 };//Assuming image format is 4 bbp
        size_t const bufferSize{ imageDescriptor.dimensions.x * imageDescriptor.dimensions.y * bytesPerPixel };
        renderTargetBuffer = *factory->createBuffer(GraphicsBuffer::Descriptor{
            .size        = bufferSize,
            .usageFlags  = GraphicsBuffer::UsageMode::TransferDestination,
            .sharingMode = SharingMode::Exclusive,
            .memoryType  = MemoryType::SystemMemory,
        });

        //Pre-record the transfer command
        ImageMemoryBarrierInfo constexpr layoutTransferInfo{
            .currentAccess      = AccessFlags::None,
            .newAccess          = AccessFlags::TransferRead,
            .currentImageLayout = GraphicsImage::Layout::Undefined,
            .newImageLayout     = GraphicsImage::Layout::TransferSourceOptimal,
            .sourceQueue        = QueueType::None,
            .destinationQueue   = QueueType::None,
        };

        transferCommandBuffer->beginRecording(CommandBufferUsage::Default);
        transferCommandBuffer->imageMemoryBarrier(*renderTargetImage, std::move(layoutTransferInfo), PipelineObject::Stage::Top, PipelineObject::Stage::Transfer);
        transferCommandBuffer->copyImageToBuffer(*renderTargetImage, { 0, 0, 0 }, { imageDescriptor.dimensions, 1 }, *renderTargetBuffer, 0);
        transferCommandBuffer->endRecording();

        onPropertiesChangedEnd.broadcast();
    }
}