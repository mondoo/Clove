#pragma once

//TODO: Remove
#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
	class VKSwapchain {
		//VARIABLES
	private:
		VkSwapchainKHR swapChain = VK_NULL_HANDLE;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKSwapchain();
		~VKSwapchain();

		//TODO: a 'present' function, takes the presentation queue
	};
}