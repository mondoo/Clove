#pragma once

//TODO: Remove
#include <vulkan/vulkan.h>
#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"

namespace clv::gfx {
	class FramebufferDescriptor {
		
	};
}

namespace clv::gfx::vk {
	class VKFramebuffer {
		//VARIABLES
	private:
		VkDevice device = VK_NULL_HANDLE;

		VkFramebuffer framebuffer = VK_NULL_HANDLE;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKFramebuffer(VkDevice device, FramebufferDescriptor descriptor);
		~VKFramebuffer();
	};
}