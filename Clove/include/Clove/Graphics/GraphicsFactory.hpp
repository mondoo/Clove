#pragma once

#include "Clove/Graphics/Fence.hpp"
#include "Clove/Graphics/GraphicsBuffer.hpp"
#include "Clove/Graphics/GraphicsImage.hpp"
#include "Clove/Graphics/Semaphore.hpp"

namespace clv::gfx {
    /**
     * @brief Used to create graphics objects. Abstracts away the underlying API.
     */
    class GraphicsFactory {
        //FUNCTIONS
    public:
        virtual ~GraphicsFactory() = default;

        //std::unique_ptr<VKGraphicsQueue> createGraphicsQueue(CommandQueueDescriptor descriptor);
        //std::unique_ptr<VKPresentQueue> createPresentQueue();
        //std::unique_ptr<VKTransferQueue> createTransferQueue(CommandQueueDescriptor descriptor);

        //std::unique_ptr<VKSwapchain> createSwapChain(SwapchainDescriptor descriptor);

        //std::unique_ptr<VKShader> createShader(std::string_view filePath);
        //std::unique_ptr<VKShader> createShader(std::vector<std::byte> byteCode);

        //std::unique_ptr<VKRenderPass> createRenderPass(RenderPassDescriptor descriptor);
        //std::unique_ptr<VKDescriptorSetLayout> createDescriptorSetLayout(DescriptorSetLayoutDescriptor descriptor);

        //std::unique_ptr<VKPipelineObject> createPipelineObject(PiplineObjectDescriptor descriptor);

        //std::unique_ptr<VKFramebuffer> createFramebuffer(FramebufferDescriptor descriptor);
        //std::unique_ptr<VKDescriptorPool> createDescriptorPool(DescriptorPoolDescriptor descriptor);

        virtual std::unique_ptr<Semaphore> createSemaphore() = 0;
        virtual std::unique_ptr<Fence> createFence(Fence::Descriptor descriptor) = 0;

        virtual std::unique_ptr<GraphicsBuffer> createBuffer(GraphicsBuffer::Descriptor descriptor) = 0;
        virtual std::unique_ptr<GraphicsImage> createImage(GraphicsImage::Descriptor descriptor)    = 0;

        //std::unique_ptr<VKSampler> createSampler(SamplerDescriptor descriptor);

        //void waitForIdleDevice();
    };
}