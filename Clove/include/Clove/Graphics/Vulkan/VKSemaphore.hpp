#pragma once

//TODO: Remove
#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
	class VKSemaphore {
		//VARIABLES
	private:
		VkDevice device;
		VkSemaphore semaphore;

		//FUNCTIONS
	public:
		//TODO: Other Ctors
		VKSemaphore(VkDevice device);
		~VKSemaphore();
	};
}