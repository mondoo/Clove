#pragma once

//TODO: Remove to cpp
#include "Clove/Graphics/Vulkan/VKCommandQueue.hpp"
#include "Clove/Graphics/Vulkan/VKSwapChain.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

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

		VkExtent2D windowExtent;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKGraphicsFactory(void* nativeWindow);
		~VKGraphicsFactory();

		std::unique_ptr<VKGraphicsQueue> createGraphicsQueue(CommandQueueDescriptor descriptor);
		std::unique_ptr<VKPresentQueue> createPresentQueue();
		std::unique_ptr<VKTransferQueue> createTransferQueue(CommandQueueDescriptor descriptor);

		std::unique_ptr<VKSwapchain> createSwapChain(SwapchainDescriptor descriptor);
	};
}