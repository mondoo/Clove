#include "Clove/Rendering/RenderGraph/RgFrameCache.hpp"

namespace garlic::clove {
    RgFrameCache::RgFrameCache(std::shared_ptr<GhaFactory> ghaFactory, GhaGraphicsQueue &graphicsQueue, GhaComputeQueue &computeQueue, GhaTransferQueue &transferQueue)
        : ghaFactory{ std::move(ghaFactory) } {
        graphicsCommandBuffer = graphicsQueue.allocateCommandBuffer();
        computeCommandBuffer  = computeQueue.allocateCommandBuffer();
        transferCommandBuffer = transferQueue.allocateCommandBuffer();
    }

    RgFrameCache::RgFrameCache(RgFrameCache &&other) noexcept = default;

    RgFrameCache &RgFrameCache::operator=(RgFrameCache &&other) noexcept = default;

    RgFrameCache::~RgFrameCache() = default;

    void RgFrameCache::reset() {
        bufferPool.reset();
        imagePool.reset();
        framebufferPool.reset();
        descriptorPoolPool.reset();
    }

    std::shared_ptr<GhaBuffer> RgFrameCache::allocateBuffer(GhaBuffer::Descriptor descriptor) {
        //TODO
        return nullptr;
    }

    std::shared_ptr<GhaImage> RgFrameCache::allocateImage(GhaImage::Descriptor descriptor) {
        //TODO
        return nullptr;
    }

    std::shared_ptr<GhaFramebuffer> RgFrameCache::allocateFrameBuffer(GhaFramebuffer::Descriptor descriptor) {
        //TODO
        return nullptr;
    }

    std::shared_ptr<GhaDescriptorPool> RgFrameCache::allocateDescriptorPool(GhaDescriptorPool::Descriptor descriptor) {
        //TODO
        return nullptr;
    }

    std::shared_ptr<GhaGraphicsCommandBuffer> RgFrameCache::getGraphicsCommandBuffer() {
        //TODO: Keep a single one?
        return nullptr;
    }

    std::shared_ptr<GhaComputeCommandBuffer> RgFrameCache::getComputeCommandBuffer() {
        //TODO: Keep a single one?
        return nullptr;
    }

    std::shared_ptr<GhaTransferCommandBuffer> RgFrameCache::getTransferCommandBuffer() {
        //TODO: Keep a single one?
        return nullptr;
    }
}