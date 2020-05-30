#pragma once

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
	class VKGraphicsFactory {
		//VARIABLES
	private:
		VkDebugUtilsMessengerEXT debugMessenger;

		VkInstance instance;

		VkSurfaceKHR surface;

		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKGraphicsFactory(void* nativeWindow);
		~VKGraphicsFactory();
	};
}