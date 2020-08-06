#pragma once

#include "Clove/Graphics/GraphicsFactory.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKGraphicsFactory : public GraphicsFactory {
        //VARIABLES
    private:
        VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;

        VkInstance instance = VK_NULL_HANDLE;

        VkSurfaceKHR surface = VK_NULL_HANDLE;

        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice logicalDevice          = VK_NULL_HANDLE;

        QueueFamilyIndices queueFamilyIndices;

        //FUNCTIONS
    public:
        VKGraphicsFactory() = delete;
        VKGraphicsFactory(void* nativeWindow);

        VKGraphicsFactory(const VKGraphicsFactory& other) = delete;
        VKGraphicsFactory(VKGraphicsFactory&& other) noexcept;

        VKGraphicsFactory& operator=(const VKGraphicsFactory& other) = delete;
        VKGraphicsFactory& operator=(VKGraphicsFactory&& other) noexcept;

        ~VKGraphicsFactory();

        std::unique_ptr<GraphicsQueue> createGraphicsQueue(CommandQueueDescriptor descriptor) override;
        std::unique_ptr<PresentQueue> createPresentQueue() override;
        std::unique_ptr<TransferQueue> createTransferQueue(CommandQueueDescriptor descriptor) override;

        std::unique_ptr<Swapchain> createSwapChain(Swapchain::Descriptor descriptor) override;

        std::unique_ptr<Shader> createShader(std::string_view filePath) override;
        std::unique_ptr<Shader> createShader(std::vector<std::byte> byteCode) override;

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

        void waitForIdleDevice() override;
    };
}