#include "Clove/Rendering/RenderGraph/RgFrameCache.hpp"

namespace garlic::clove {
    RgFrameCache::RgFrameCache(std::shared_ptr<GhaGraphicsQueue> graphicsQueue) {
        //TODO
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
}