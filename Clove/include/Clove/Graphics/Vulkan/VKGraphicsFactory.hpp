#pragma once

#include "Clove/Graphics/GraphicsFactory.hpp"

//TODO: Remove to cpp
#include "Clove/Graphics/Vulkan/VKCommandQueue.hpp"
#include "Clove/Graphics/Vulkan/VKDescriptorPool.hpp"
#include "Clove/Graphics/Vulkan/VKDescriptorSetLayout.hpp"
#include "Clove/Graphics/Vulkan/VKFramebuffer.hpp"
#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"
#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"
#include "Clove/Graphics/Vulkan/VKSampler.hpp"
#include "Clove/Graphics/Vulkan/VKSwapChain.hpp"
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
        //TODO: Ctors
        VKGraphicsFactory(void* nativeWindow);
        ~VKGraphicsFactory();

        std::unique_ptr<VKGraphicsQueue> createGraphicsQueue(CommandQueueDescriptor descriptor);
        std::unique_ptr<VKPresentQueue> createPresentQueue();
        std::unique_ptr<VKTransferQueue> createTransferQueue(CommandQueueDescriptor descriptor);

        std::unique_ptr<VKSwapchain> createSwapChain(SwapchainDescriptor descriptor);

        std::unique_ptr<Shader> createShader(std::string_view filePath) override;
        std::unique_ptr<Shader> createShader(std::vector<std::byte> byteCode) override;

        std::unique_ptr<VKRenderPass> createRenderPass(RenderPassDescriptor descriptor);
        std::unique_ptr<VKDescriptorSetLayout> createDescriptorSetLayout(DescriptorSetLayoutDescriptor descriptor);

        std::unique_ptr<VKPipelineObject> createPipelineObject(PiplineObjectDescriptor descriptor);

        std::unique_ptr<VKFramebuffer> createFramebuffer(FramebufferDescriptor descriptor);
        std::unique_ptr<VKDescriptorPool> createDescriptorPool(DescriptorPoolDescriptor descriptor);

        std::unique_ptr<Semaphore> createSemaphore() override;
        std::unique_ptr<Fence> createFence(Fence::Descriptor descriptor) override;

        std::unique_ptr<GraphicsBuffer> createBuffer(GraphicsBuffer::Descriptor descriptor) override;
        std::unique_ptr<GraphicsImage> createImage(GraphicsImage::Descriptor descriptor) override;

        std::unique_ptr<VKSampler> createSampler(SamplerDescriptor descriptor);

        void waitForIdleDevice();
    };
}