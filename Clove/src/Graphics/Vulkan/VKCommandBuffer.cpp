#include "Clove/Graphics/Vulkan/VKCommandBuffer.hpp"

namespace clv::gfx::vk {
	static VkCommandBufferUsageFlags getCommandBufferUsageFlags(CommandBufferUsage garlicUsage) {
        switch(garlicUsage) {
            case clv::gfx::CommandBufferUsage::Default:
                return 0;
            case clv::gfx::CommandBufferUsage::OneTimeSubmit:
                return VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            default:
                CLV_ASSERT(false, "{0}: Unkown usage type", CLV_FUNCTION_NAME);
                return 0;
        }
    }

	VKGraphicsCommandBuffer::VKGraphicsCommandBuffer(VkCommandBuffer commandBuffer)
		: commandBuffer(commandBuffer) {
	}

	void VKGraphicsCommandBuffer::beginRecording(CommandBufferUsage usageFlag) {
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType			   = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags			   = getCommandBufferUsageFlags(usageFlag);
		beginInfo.pInheritanceInfo = nullptr;

		if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to begin recording command buffer");
		}
	}

	void VKGraphicsCommandBuffer::beginRenderPass(VKRenderPass& renderPass, VKFramebuffer& frameBuffer, const RenderArea& renderArea, const mth::vec4f& clearColour) {
		VkClearValue clearValue = { clearColour.r, clearColour.g, clearColour.b, clearColour.a };

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType			 = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass		 = renderPass.getRenderPass();
		renderPassInfo.framebuffer		 = frameBuffer.getFrameBuffer();
		renderPassInfo.renderArea.offset = { renderArea.origin.x, renderArea.origin.y };
		renderPassInfo.renderArea.extent = { renderArea.size.x, renderArea.size.y };
		renderPassInfo.clearValueCount	 = 1;
		renderPassInfo.pClearValues		 = &clearValue;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VKGraphicsCommandBuffer::bindVertexBuffer(VKBuffer& vertexBuffer) {
        VkBuffer buffers[] = { vertexBuffer.getBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

	void VKGraphicsCommandBuffer::bindPipelineObject(VKPipelineObject& pipelineObject) {
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineObject.getPipeline());
	}

	void VKGraphicsCommandBuffer::drawIndexed(const size_t vertexCount) {
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}

	void VKGraphicsCommandBuffer::endRenderPass() {
		vkCmdEndRenderPass(commandBuffer);
	}

	void VKGraphicsCommandBuffer::endRecording() {
		if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to end recording command buffer");
		}
	}

	VkCommandBuffer VKGraphicsCommandBuffer::getCommandBuffer() const {
		return commandBuffer;
	}
}