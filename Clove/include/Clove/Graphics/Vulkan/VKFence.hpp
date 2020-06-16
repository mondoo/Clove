#pragma once

//TODO: Remove
#include <vulkan/vulkan.h>

namespace clv::gfx{
	struct FenceDescriptor{
		bool createSignaled;
	};
}

namespace clv::gfx::vk {
	class VKFence{
		//VARIABLES
	private:
		VkDevice device;
		VkFence fence;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKFence(VkDevice device, FenceDescriptor desccriptor);
		~VKFence();

		void waitForFence();
		void resetFence();
	};
}