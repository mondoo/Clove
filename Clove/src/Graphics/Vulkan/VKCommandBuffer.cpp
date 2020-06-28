#include "Clove/Graphics/Vulkan/VKCommandBuffer.hpp"

namespace clv::gfx::vk {
	VKCommandBuffer::VKCommandBuffer(VkCommandBuffer commandBuffer)
		: commandBuffer(commandBuffer) {
	}

	void VKCommandBuffer::beginRecording() {
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType			   = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags			   = 0;
		beginInfo.pInheritanceInfo = nullptr;

		if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to begin recording command buffer");
		}
	}

	void VKCommandBuffer::beginRenderPass(VKRenderPass& renderPass, VKFramebuffer& frameBuffer, const RenderArea& renderArea, const mth::vec4f& clearColour) {
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

	void VKCommandBuffer::bindVertexBuffer(VKBuffer& vertexBuffer) {
        VkBuffer buffers[] = { vertexBuffer.getBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

	void VKCommandBuffer::bindPipelineObject(VKPipelineObject& pipelineObject) {
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineObject.getPipeline());
	}

	void VKCommandBuffer::drawIndexed(const size_t vertexCount) {
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}

	void VKCommandBuffer::endRenderPass() {
		vkCmdEndRenderPass(commandBuffer);
	}

	void VKCommandBuffer::endRecording() {
		if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to end recording command buffer");
		}
	}

	VkCommandBuffer VKCommandBuffer::getCommandBuffer() const {
		return commandBuffer;
	}
}