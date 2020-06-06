#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"

namespace clv::gfx::vk {
	VKRenderPass::VKRenderPass(VkDevice device, VKSwapchain& swapchain, RenderPassDescriptor descriptor)
		: device(device) {
		//TODO: Hard coding in the attachments for now
		VkAttachmentDescription colourAttachment{};
		colourAttachment.format			= swapchain.getFormat();
		colourAttachment.samples		= VK_SAMPLE_COUNT_1_BIT;
		colourAttachment.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
		colourAttachment.storeOp		= VK_ATTACHMENT_STORE_OP_STORE;
		colourAttachment.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colourAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colourAttachment.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
		colourAttachment.finalLayout	= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colourAttachmentReference{};
		colourAttachmentReference.attachment = 0;
		colourAttachmentReference.layout	 = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint	 = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments	 = &colourAttachmentReference;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType		   = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments	   = &colourAttachment;
		renderPassInfo.subpassCount	   = 1;
		renderPassInfo.pSubpasses	   = &subpass;

		if(vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create render pass");
		}
	}

	VKRenderPass::~VKRenderPass() {
		vkDestroyRenderPass(device, renderPass, nullptr);
	}

	VkRenderPass VKRenderPass::getRenderPass() const {
		return renderPass;
	}
}