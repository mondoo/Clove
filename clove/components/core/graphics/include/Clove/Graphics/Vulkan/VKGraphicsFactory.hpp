#pragma once

#include "Clove/Graphics/GraphicsFactory.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include <optional>
#include <span>
#include <vulkan/vulkan.h>

namespace garlic::clove {
    class MemoryAllocator;
}

namespace garlic::clove {
    class VKGraphicsFactory : public GraphicsFactory {
        //VARIABLES
    private:
        DevicePointer devicePtr;

        QueueFamilyIndices queueFamilyIndices;
        std::optional<SwapchainSupportDetails> swapchainSupportDetails;

        std::shared_ptr<MemoryAllocator> memoryAllocator;

        //FUNCTIONS
    public:
        VKGraphicsFactory() = delete;
        VKGraphicsFactory(DevicePointer devicePtr, QueueFamilyIndices queueFamilyIndices, std::optional<SwapchainSupportDetails> swapchainSupportDetails);

        VKGraphicsFactory(VKGraphicsFactory const &other) = delete;
        VKGraphicsFactory(VKGraphicsFactory &&other) noexcept;

        VKGraphicsFactory &operator=(VKGraphicsFactory const &other) = delete;
        VKGraphicsFactory &operator=(VKGraphicsFactory &&other) noexcept;

        ~VKGraphicsFactory();

        std::unique_ptr<GraphicsQueue> createGraphicsQueue(CommandQueueDescriptor descriptor) override;
        Expected<std::unique_ptr<PresentQueue>, std::string> createPresentQueue() override;
        std::unique_ptr<TransferQueue> createTransferQueue(CommandQueueDescriptor descriptor) override;

        Expected<std::unique_ptr<Swapchain>, std::string> createSwapChain(Swapchain::Descriptor descriptor) override;

        std::unique_ptr<Shader> createShader(std::string_view filePath) override;
        std::unique_ptr<Shader> createShader(std::span<std::byte const> byteCode) override;

        std::unique_ptr<RenderPass> createRenderPass(RenderPass::Descriptor descriptor) override;
        std::unique_ptr<DescriptorSetLayout> createDescriptorSetLayout(DescriptorSetLayout::Descriptor descriptor) override;

        std::unique_ptr<PipelineObject> createPipelineObject(PipelineObject::Descriptor descriptor) override;

        std::unique_ptr<Framebuffer> createFramebuffer(Framebuffer::Descriptor descriptor) override;
        std::unique_ptr<DescriptorPool> createDescriptorPool(DescriptorPool::Descriptor descriptor) override;

        std::unique_ptr<Semaphore> createSemaphore() override;
        std::unique_ptr<Fence> createFence(Fence::Descriptor descriptor) override;

        std::unique_ptr<GraphicsBuffer> createBuffer(GraphicsBuffer::Descriptor descriptor) override;
        std::unique_ptr<GraphicsImage> createImage(GraphicsImage::Descriptor descriptor) override;

        std::unique_ptr<Sampler> createSampler(Sampler::Descriptor descriptor) override;
    };
}