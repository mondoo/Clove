#pragma once

//TODO: Remove
#include <vulkan/vulkan.h>
#include "Clove/Graphics/Vulkan/VKSwapchain.hpp"

namespace clv::gfx {
	struct RenderPassDescriptor {
	};
}

namespace clv::gfx::vk {
	class VKRenderPass {
		//VARIABLES
	private:
		VkDevice device = VK_NULL_HANDLE;

		VkRenderPass renderPass = VK_NULL_HANDLE;

		//FUNCTIONS
	public:
		//TODO: ctors
		//TODO: Swapcahin is temp
		VKRenderPass(VkDevice device, VKSwapchain& swapchain, RenderPassDescriptor descriptor);
		~VKRenderPass();

		VkRenderPass getRenderPass() const;
	};
}