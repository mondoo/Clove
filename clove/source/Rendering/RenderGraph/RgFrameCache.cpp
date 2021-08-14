#include "Clove/Rendering/RenderGraph/RgFrameCache.hpp"

#include "Clove/Rendering/RenderGraph/CacheUtils.hpp"

#include <Clove/Log/Log.hpp>
#include <functional>

namespace garlic::clove {
    RgFrameCache::RgFrameCache(std::shared_ptr<GhaFactory> ghaFactory, std::shared_ptr<GhaGraphicsQueue> graphicsQueue, std::shared_ptr<GhaComputeQueue> computeQueue, std::shared_ptr<GhaTransferQueue> transferQueue)
        : ghaFactory{ std::move(ghaFactory) }
        , graphicsQueue{ std::move(graphicsQueue) }
        , computeQueue{ std::move(computeQueue) }
        , transferQueue{ std::move(transferQueue) } {
    }

    RgFrameCache::RgFrameCache(RgFrameCache &&other) noexcept = default;

    RgFrameCache &RgFrameCache::operator=(RgFrameCache &&other) noexcept = default;

    RgFrameCache::~RgFrameCache() = default;

    void RgFrameCache::reset() {
        bufferPool.reset();
        imagePool.reset();
        imageViewPool.reset();
        framebufferPool.reset();
        descriptorPoolPool.reset();
        semaphorePool.reset();

        for(auto const &commandBuffer : allocatedGraphicsCommandBuffers) {
            graphicsQueue->freeCommandBuffer(*commandBuffer);
        }
        allocatedGraphicsCommandBuffers.clear();

        for(auto const &commandBuffer : allocatedComputeCommandBuffers) {
            computeQueue->freeCommandBuffer(*commandBuffer);
        }
        allocatedComputeCommandBuffers.clear();

        for(auto const &commandBuffer : allocatedTransferCommandBuffers) {
            transferQueue->freeCommandBuffer(*commandBuffer);
        }
        allocatedTransferCommandBuffers.clear();
    }

    std::shared_ptr<GhaBuffer> RgFrameCache::allocateBuffer(GhaBuffer::Descriptor descriptor) {
        PoolId bufferId{ 0 };
        CacheUtils::hashCombine(bufferId, descriptor.size);
        CacheUtils::hashCombine(bufferId, descriptor.usageFlags);
        CacheUtils::hashCombine(bufferId, descriptor.sharingMode);
        CacheUtils::hashCombine(bufferId, descriptor.memoryType);

        return bufferPool.retrieve(bufferId, [&]() {
            return ghaFactory->createBuffer(std::move(descriptor)).getValue();
        });
    }

    std::shared_ptr<GhaImage> RgFrameCache::allocateImage(GhaImage::Descriptor descriptor) {
        PoolId imageId{ 0 };
        CacheUtils::hashCombine(imageId, descriptor.type);
        CacheUtils::hashCombine(imageId, descriptor.usageFlags);
        CacheUtils::hashCombine(imageId, descriptor.dimensions.x);
        CacheUtils::hashCombine(imageId, descriptor.dimensions.y);
        CacheUtils::hashCombine(imageId, descriptor.format);
        CacheUtils::hashCombine(imageId, descriptor.sharingMode);

        return imagePool.retrieve(imageId, [&]() {
            return ghaFactory->createImage(std::move(descriptor)).getValue();
        });
    }

    std::shared_ptr<GhaImageView> RgFrameCache::allocateImageView(GhaImage const *const image, GhaImageView::Descriptor descriptor) {
        PoolId viewId{ 0 };
        CacheUtils::hashCombine(viewId, image);
        CacheUtils::hashCombine(viewId, descriptor.type);
        CacheUtils::hashCombine(viewId, descriptor.layer);
        CacheUtils::hashCombine(viewId, descriptor.layerCount);

        return imageViewPool.retrieve(viewId, [&]() {
            return ghaFactory->createImageView(*image, std::move(descriptor)).getValue();
        });
    }

    std::shared_ptr<GhaFramebuffer> RgFrameCache::allocateFramebuffer(GhaFramebuffer::Descriptor descriptor) {
        PoolId framebufferId{ 0 };
        CacheUtils::hashCombine(framebufferId, descriptor.renderPass.get());
        for(auto const &imageView : descriptor.attachments) {
            CacheUtils::hashCombine(framebufferId, imageView.get());
        }
        CacheUtils::hashCombine(framebufferId, descriptor.width);
        CacheUtils::hashCombine(framebufferId, descriptor.height);

        return framebufferPool.retrieve(framebufferId, [&]() {
            return ghaFactory->createFramebuffer(std::move(descriptor)).getValue();
        });
    }

    std::shared_ptr<GhaDescriptorPool> RgFrameCache::allocateDescriptorPool(GhaDescriptorPool::Descriptor descriptor) {
        PoolId poolId{ 0 };
        for(auto &poolType : descriptor.poolTypes) {
            CacheUtils::hashCombine(poolId, poolType.type);
            CacheUtils::hashCombine(poolId, poolType.count);
        }
        CacheUtils::hashCombine(poolId, descriptor.flag);
        CacheUtils::hashCombine(poolId, descriptor.maxSets);

        return descriptorPoolPool.retrieve(poolId, [&]() {
            return ghaFactory->createDescriptorPool(std::move(descriptor)).getValue();
        });
    }

    std::shared_ptr<GhaSemaphore> RgFrameCache::allocateSemaphore() {
        //Always assign a new one until reset.
        static PoolId poolId{ 0 };
        ++poolId;

        return semaphorePool.retrieve(poolId, [&]() {
            return ghaFactory->createSemaphore().getValue();
        });
    }

    std::shared_ptr<GhaGraphicsCommandBuffer> RgFrameCache::allocateGraphicsCommandBuffer() {
        std::shared_ptr<GhaGraphicsCommandBuffer> commandBuffer{ graphicsQueue->allocateCommandBuffer() };
        allocatedGraphicsCommandBuffers.push_back(commandBuffer);
        return commandBuffer;
    }

    std::shared_ptr<GhaComputeCommandBuffer> RgFrameCache::allocateComputeCommandBuffer() {
        std::shared_ptr<GhaComputeCommandBuffer> commandBuffer{ computeQueue->allocateCommandBuffer() };
        allocatedComputeCommandBuffers.push_back(commandBuffer);
        return commandBuffer;
    }

    std::shared_ptr<GhaTransferCommandBuffer> RgFrameCache::allocateTransferCommandBuffer() {
        std::shared_ptr<GhaTransferCommandBuffer> commandBuffer{ transferQueue->allocateCommandBuffer() };
        allocatedTransferCommandBuffers.push_back(commandBuffer);
        return commandBuffer;
    }

    void RgFrameCache::submit(GraphicsSubmitInfo submitInfo, GhaFence *signalFence) {
        graphicsQueue->submit({ std::move(submitInfo) }, signalFence);
    }

    void RgFrameCache::submit(ComputeSubmitInfo submitInfo, GhaFence *signalFence) {
        computeQueue->submit({ std::move(submitInfo) }, signalFence);
    }

    void RgFrameCache::submit(TransferSubmitInfo submitInfo, GhaFence *signalFence) {
        transferQueue->submit({ std::move(submitInfo) }, signalFence);
    }
}