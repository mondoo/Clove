#include "Clove/Rendering/RenderGraph/RgFrameCache.hpp"

#include "Clove/Rendering/RenderGraph/CacheUtils.hpp"

#include <Clove/Log/Log.hpp>
#include <functional>

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
        PoolId bufferId{ 0 };
        CacheUtils::hashCombine(bufferId, descriptor.size);
        CacheUtils::hashCombine(bufferId, descriptor.usageFlags);
        CacheUtils::hashCombine(bufferId, descriptor.sharingMode);
        CacheUtils::hashCombine(bufferId, descriptor.memoryType);

        if(bufferPool.free.contains(bufferId)) {
            bufferPool.allocated.insert(bufferPool.free.extract(bufferId));
        } else {
            bufferPool.allocated.emplace(bufferId, ghaFactory->createBuffer(std::move(descriptor)).getValue());
        }

        return bufferPool.allocated.find(bufferId)->second;
    }

    std::shared_ptr<GhaImage> RgFrameCache::allocateImage(GhaImage::Descriptor descriptor) {
        PoolId imageId{ 0 };
        CacheUtils::hashCombine(imageId, descriptor.type);
        CacheUtils::hashCombine(imageId, descriptor.usageFlags);
        CacheUtils::hashCombine(imageId, descriptor.dimensions.x);
        CacheUtils::hashCombine(imageId, descriptor.dimensions.y);
        CacheUtils::hashCombine(imageId, descriptor.format);
        CacheUtils::hashCombine(imageId, descriptor.sharingMode);

        if(imagePool.free.contains(imageId)) {
            imagePool.allocated.insert(imagePool.free.extract(imageId));
        } else {
            imagePool.allocated.emplace(imageId, ghaFactory->createImage(std::move(descriptor)).getValue());
        }

        return imagePool.allocated.find(imageId)->second;
    }

    std::shared_ptr<GhaFramebuffer> RgFrameCache::allocateFramebuffer(GhaFramebuffer::Descriptor descriptor) {
        auto const &renderPassDesc{ descriptor.renderPass->getDescriptor() };
        auto const hashAttachment = [](PoolId &id, AttachmentDescriptor const &attachment) {
            CacheUtils::hashCombine(id, attachment.format);
            CacheUtils::hashCombine(id, attachment.loadOperation);
            CacheUtils::hashCombine(id, attachment.storeOperation);
            CacheUtils::hashCombine(id, attachment.initialLayout);
            CacheUtils::hashCombine(id, attachment.usedLayout);
            CacheUtils::hashCombine(id, attachment.finalLayout);
        };

        PoolId framebufferId{ 0 };
        for(auto &attachment : renderPassDesc.colourAttachments){
            hashAttachment(framebufferId, attachment);
        }
        hashAttachment(framebufferId, renderPassDesc.depthAttachment);
        CacheUtils::hashCombine(framebufferId, descriptor.attachments.size());
        CacheUtils::hashCombine(framebufferId, descriptor.width);
        CacheUtils::hashCombine(framebufferId, descriptor.height);

        if(framebufferPool.free.contains(framebufferId)) {
            framebufferPool.allocated.insert(framebufferPool.free.extract(framebufferId));
        } else {
            framebufferPool.allocated.emplace(framebufferId, ghaFactory->createFramebuffer(std::move(descriptor)).getValue());
        }

        return framebufferPool.allocated.find(framebufferId)->second;
    }

    std::shared_ptr<GhaDescriptorPool> RgFrameCache::allocateDescriptorPool(GhaDescriptorPool::Descriptor descriptor) {
        PoolId poolId{ 0 };
        for(auto &poolType : descriptor.poolTypes){
            CacheUtils::hashCombine(poolId, poolType.type);
            CacheUtils::hashCombine(poolId, poolType.count);
        }
        CacheUtils::hashCombine(poolId, descriptor.flag);
        CacheUtils::hashCombine(poolId, descriptor.maxSets);

        if(descriptorPoolPool.free.contains(poolId)) {
            descriptorPoolPool.allocated.insert(descriptorPoolPool.free.extract(poolId));
        } else {
            descriptorPoolPool.allocated.emplace(poolId, ghaFactory->createDescriptorPool(std::move(descriptor)).getValue());
        }

        return descriptorPoolPool.allocated.find(poolId)->second;
    }

    std::shared_ptr<GhaGraphicsCommandBuffer> RgFrameCache::getGraphicsCommandBuffer() {
        return graphicsCommandBuffer;
    }

    std::shared_ptr<GhaComputeCommandBuffer> RgFrameCache::getComputeCommandBuffer() {
        return computeCommandBuffer;
    }

    std::shared_ptr<GhaTransferCommandBuffer> RgFrameCache::getTransferCommandBuffer() {
        return transferCommandBuffer;
    }
}