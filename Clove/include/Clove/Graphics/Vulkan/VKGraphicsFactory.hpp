#pragma once

#include <vulkan/vulkan.h>

//TODO: Remove
#include "Clove/Graphics/Vulkan/VKSwapChain.hpp"

namespace clv::gfx::vk {
	class VKGraphicsFactory {
		//VARIABLES
	private:
		VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;

		VkInstance instance = VK_NULL_HANDLE;

		VkSurfaceKHR surface = VK_NULL_HANDLE;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice logicalDevice = VK_NULL_HANDLE;

		//TODO: Move these out into an abstraction
		VkQueue graphicsQueue = VK_NULL_HANDLE;
		VkQueue transferQueue = VK_NULL_HANDLE;
		VkQueue presentQueue = VK_NULL_HANDLE;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKGraphicsFactory(void* nativeWindow);
		~VKGraphicsFactory();

		std::unique_ptr<VKSwapchain> createSwapChain();
	};
}