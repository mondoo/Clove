#pragma once

//TODO: Remove
#include <vulkan/vulkan.h>
#include "Clove/Graphics/Vulkan/VKSwapchain.hpp"

#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

namespace clv::gfx {
	struct RenderPassDescriptor {
		ImageFormat imageFormat{ ImageFormat::Unkown };
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
		VKRenderPass(VkDevice device, RenderPassDescriptor descriptor);
		~VKRenderPass();

		//VK specific
		VkRenderPass getRenderPass() const;
	};
}