#pragma once

#include <Clove/Graphics/GhaBuffer.hpp>
#include <Clove/Graphics/GhaDescriptorPool.hpp>
#include <Clove/Graphics/GhaFramebuffer.hpp>
#include <Clove/Graphics/GhaGraphicsCommandBuffer.hpp>
#include <Clove/Graphics/GhaComputeCommandBuffer.hpp>
#include <Clove/Graphics/GhaTransferCommandBuffer.hpp>
#include <Clove/Graphics/GhaImage.hpp>
#include <Clove/Graphics/GhaDescriptorPool.hpp>
#include <memory>
#include <vector>

namespace garlic::clove {
    /**
     * @brief Stores objects associated with a single frame (images, buffers, etc.) for a RenderGraph
     */
    class RgFrameCache {
        //VARIABLES
    private:
        std::vector<std::shared_ptr<GhaBuffer>> buffers{};

        //FUNCTIONS
    public:
        //TODO: ctors
        RgFrameCache(std::shared_ptr<GhaGraphicsQueue> graphicsQueue);

        std::shared_ptr<GhaBuffer> allocateBuffer(GhaBuffer::Descriptor descriptor);
        std::shared_ptr<GhaImage> allocateImage(GhaImage::Descriptor descriptor);

        std::shared_ptr<GhaFramebuffer> allocateFrameBuffer(GhaFramebuffer::Descriptor descriptor);

        std::shared_ptr<GhaDescriptorPool> allocateDescriptorPool(GhaDescriptorPool::Descriptor descriptor);

        std::shared_ptr<GhaGraphicsCommandBuffer> getGraphicsCommandBuffer();
        std::shared_ptr<GhaComputeCommandBuffer> getComputeCommandBuffer();
        std::shared_ptr<GhaTransferCommandBuffer> getTransferCommandBuffer();
    };
}