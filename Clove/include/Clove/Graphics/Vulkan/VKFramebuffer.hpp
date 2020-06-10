#pragma once

//TODO: Remove
#include "Clove/Graphics/Vulkan/VKImageView.hpp"
#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx {
	struct FramebufferDescriptor {
		std::shared_ptr<vk::VKRenderPass> renderPass;
		std::vector<std::shared_ptr<vk::VKImageView>> attachments;
		uint32_t width	= 0;
		uint32_t height = 0;
	};
}

namespace clv::gfx::vk {
	class VKFramebuffer {
		//VARIABLES
	private:
		VkDevice device = VK_NULL_HANDLE;

		VkFramebuffer framebuffer = VK_NULL_HANDLE;

		FramebufferDescriptor descriptor;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKFramebuffer(VkDevice device, FramebufferDescriptor descriptor);
		~VKFramebuffer();

		VkFramebuffer getFrameBuffer() const {
			return framebuffer;
		}
	};
}