#pragma once

//TODO: Remove
#include <vulkan/vulkan.h>
#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"

namespace clv::gfx::vk {
	class VKCommandBuffer {
		//VARIABLES
	private:
		VkCommandBuffer commandBuffer = VK_NULL_HANDLE;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKCommandBuffer(VkCommandBuffer commandBuffer);
	};
}