#include "Clove/Graphics/Vulkan/VKFramebuffer.hpp"

namespace clv::gfx::vk {
	VKFramebuffer::VKFramebuffer(VkDevice device, FramebufferDescriptor descriptor)
		: device(device) {

	}

	VKFramebuffer::~VKFramebuffer() {
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}
}