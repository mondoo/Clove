#include "Clove/Graphics/Vulkan/VKFramebuffer.hpp"

namespace clv::gfx::vk {
	VKFramebuffer::VKFramebuffer(VkDevice device, FramebufferDescriptor descriptor)
		: device(device)
		, descriptor(std::move(descriptor)) {
		std::vector<VkImageView> attachments;
		attachments.reserve(std::size(this->descriptor.attachments));
		for(auto& attachment : this->descriptor.attachments) {
			attachments.push_back(attachment->getImageView());
		}

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType			= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass		= this->descriptor.renderPass->getRenderPass();
		framebufferInfo.attachmentCount = std::size(attachments);
		framebufferInfo.pAttachments	= std::data(attachments);
		framebufferInfo.width			= this->descriptor.width;
		framebufferInfo.height			= this->descriptor.height;
		framebufferInfo.layers			= 1;

		if(vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffer) != VK_SUCCESS) {
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create framebuffer");
		}
	}

	VKFramebuffer::~VKFramebuffer() {
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}
}