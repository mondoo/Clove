#pragma once

//TODO: Remove to cpp
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"
#include "Clove/Graphics/Vulkan/VKCommandQueue.hpp"
#include "Clove/Graphics/Vulkan/VKFramebuffer.hpp"
#include "Clove/Graphics/Vulkan/VKSwapChain.hpp"
#include "Clove/Graphics/Vulkan/VKShader.hpp"
#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"
#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"
#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"
#include "Clove/Graphics/Vulkan/VKFence.hpp"
#include "Clove/Graphics/Vulkan/VKBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKDescriptorSetLayout.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
	class VKGraphicsFactory {
		//VARIABLES
	private:
		VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;

		VkInstance instance = VK_NULL_HANDLE;

		VkSurfaceKHR surface = VK_NULL_HANDLE;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice logicalDevice = VK_NULL_HANDLE;

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

		std::unique_ptr<VKShader> createShader(std::string_view filePath);
		std::unique_ptr<VKShader> createShader(std::vector<std::byte> byteCode);

		std::unique_ptr<VKRenderPass> createRenderPass(RenderPassDescriptor descriptor);
        std::unique_ptr<VKDescriptorSetLayout> createDescriptorSetLayout(DescriptorSetLayoutDescriptor descriptor);

		std::unique_ptr<VKPipelineObject> createPipelineObject(PiplineObjectDescriptor descriptor);

		std::unique_ptr<VKFramebuffer> createFramebuffer(FramebufferDescriptor descriptor);

		std::unique_ptr<VKSemaphore> createSemaphore();
		std::unique_ptr<VKFence> createFence(FenceDescriptor descriptor);

		std::unique_ptr<VKBuffer> createBuffer(BufferDescriptor2 descriptor);

		void waitForIdleDevice();
	};
}