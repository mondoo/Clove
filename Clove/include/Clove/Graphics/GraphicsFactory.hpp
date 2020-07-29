#pragma once

#include "Clove/Graphics/GraphicsBuffer.hpp"

namespace clv::gfx {
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

        //std::unique_ptr<VKSemaphore> createSemaphore();
        //std::unique_ptr<VKFence> createFence(FenceDescriptor descriptor);

        virtual std::unique_ptr<GraphicsBuffer> createBuffer(GraphicsBuffer::Descriptor descriptor) = 0;
        //std::unique_ptr<VKImage> createImage(ImageDescriptor descriptor);

        //std::unique_ptr<VKSampler> createSampler(SamplerDescriptor descriptor);

        void waitForIdleDevice();
    };
}