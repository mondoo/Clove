#include "Clove/Rendering/RenderGraph/RgFrameCache.hpp"

#include <Clove/Log/Log.hpp>
#include <functional>

namespace garlic::clove {
    namespace {
        template<typename T>
        void hasCombine(RgFrameCache::PoolId &currentHash, T const &value) {
            std::hash<T> hash{};
            currentHash ^= hash(value) + 0x9e3779b9 + (currentHash << 6) + (currentHash >> 2);
        }
    }

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
        PoolId bufferId{ 0 };
        hasCombine(bufferId, descriptor.size);
        hasCombine(bufferId, descriptor.usageFlags);
        hasCombine(bufferId, descriptor.sharingMode);
        hasCombine(bufferId, descriptor.memoryType);

        if(bufferPool.free.contains(bufferId)) {
            bufferPool.allocated.insert(bufferPool.free.extract(bufferId));
        } else {
            bufferPool.allocated.emplace(bufferId, ghaFactory->createBuffer(std::move(descriptor)).getValue());
        }

        return bufferPool.allocated.find(bufferId)->second;
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