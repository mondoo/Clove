#pragma once

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

		//TODO: Putting the queues here for now, perhaps they'll be better elsewhere?
		VkQueue graphicsQueue = VK_NULL_HANDLE;
		VkQueue transferQueue = VK_NULL_HANDLE;
		VkQueue presentQueue = VK_NULL_HANDLE;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKGraphicsFactory(void* nativeWindow);
		~VKGraphicsFactory();
	};
}