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

        Expected<std::unique_ptr<GraphicsQueue>, std::exception> createGraphicsQueue(CommandQueueDescriptor descriptor) override;
        Expected<std::unique_ptr<PresentQueue>, std::exception> createPresentQueue() override;
        Expected<std::unique_ptr<TransferQueue>, std::exception> createTransferQueue(CommandQueueDescriptor descriptor) override;

        Expected<std::unique_ptr<Swapchain>, std::exception> createSwapChain(Swapchain::Descriptor descriptor) override;

        Expected<std::unique_ptr<Shader>, std::exception> createShader(std::string_view filePath) override;
        Expected<std::unique_ptr<Shader>, std::exception> createShader(std::span<std::byte const> byteCode) override;

        Expected<std::unique_ptr<RenderPass>, std::exception> createRenderPass(RenderPass::Descriptor descriptor) override;
        Expected<std::unique_ptr<DescriptorSetLayout>, std::exception> createDescriptorSetLayout(DescriptorSetLayout::Descriptor descriptor) override;

        Expected<std::unique_ptr<PipelineObject>, std::exception> createPipelineObject(PipelineObject::Descriptor descriptor) override;

        Expected<std::unique_ptr<Framebuffer>, std::exception> createFramebuffer(Framebuffer::Descriptor descriptor) override;
        Expected<std::unique_ptr<DescriptorPool>, std::exception> createDescriptorPool(DescriptorPool::Descriptor descriptor) override;

        Expected<std::unique_ptr<Semaphore>, std::exception> createSemaphore() override;
        Expected<std::unique_ptr<Fence>, std::exception> createFence(Fence::Descriptor descriptor) override;

        Expected<std::unique_ptr<GraphicsBuffer>, std::exception> createBuffer(GraphicsBuffer::Descriptor descriptor) override;
        Expected<std::unique_ptr<GraphicsImage>, std::exception> createImage(GraphicsImage::Descriptor descriptor) override;

        Expected<std::unique_ptr<Sampler>, std::exception> createSampler(Sampler::Descriptor descriptor) override;
    };
}