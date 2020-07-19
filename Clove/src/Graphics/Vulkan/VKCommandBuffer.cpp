#include "Clove/Graphics/Vulkan/VKCommandBuffer.hpp"

#include "Clove/Graphics/Vulkan/VulkanHelpers.hpp"

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

	static VkIndexType getIndexType(IndexType garlicType){
        switch(garlicType) {
            case IndexType::Uint16:
                return VK_INDEX_TYPE_UINT16;
            default:
                CLV_ASSERT(false, "{0}: Unkown index type", CLV_FUNCTION_NAME);
                return VK_INDEX_TYPE_UINT16;
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

    void VKGraphicsCommandBuffer::endRecording() {
        if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to end recording command buffer");
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

    void VKGraphicsCommandBuffer::endRenderPass() {
        vkCmdEndRenderPass(commandBuffer);
    }

	void VKGraphicsCommandBuffer::bindVertexBuffer(VKBuffer& vertexBuffer) {
        VkBuffer buffers[] = { vertexBuffer.getBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

	void VKGraphicsCommandBuffer::bindIndexBuffer(VKBuffer& indexBuffer, IndexType indexType) {
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer.getBuffer(), 0, getIndexType(indexType));
    }

	void VKGraphicsCommandBuffer::bindPipelineObject(VKPipelineObject& pipelineObject) {
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineObject.getPipeline());
	}

	void VKGraphicsCommandBuffer::bindDescriptorSet(VKDescriptorSet& descriptorSet, const VKPipelineObject& pipeline) {
        VkDescriptorSet sets[] = { descriptorSet.getDescriptorSet() };
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getLayout(), 0, 1, sets, 0, nullptr);
    }

	void VKGraphicsCommandBuffer::drawIndexed(const size_t indexCount) {
        vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
	}

    void VKGraphicsCommandBuffer::transitionImageLayout(VKImage& image, ImageLayout previousLayout, ImageLayout newLayout) {
        VkImageMemoryBarrier barrier{};
        barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.pNext                           = nullptr;
        barrier.srcAccessMask                   = 0;//TODO
        barrier.dstAccessMask                   = 0;//TODO
        barrier.oldLayout                       = convertImageLayout(previousLayout);
        barrier.newLayout                       = convertImageLayout(newLayout);
        barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.image                           = image.getImage();
        barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;//TODO: Handle other aspect masks
        barrier.subresourceRange.baseMipLevel   = 0;
        barrier.subresourceRange.levelCount     = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = 1;
        vkCmdPipelineBarrier(commandBuffer, 0 /* TODO */, 0 /* TODO */, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    }

	VkCommandBuffer VKGraphicsCommandBuffer::getCommandBuffer() const {
		return commandBuffer;
	}

	VKTransferCommandBuffer::VKTransferCommandBuffer(VkCommandBuffer commandBuffer) 
		: commandBuffer(commandBuffer){
    }

	void VKTransferCommandBuffer::beginRecording(CommandBufferUsage usageFlag) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags            = getCommandBufferUsageFlags(usageFlag);
        beginInfo.pInheritanceInfo = nullptr;

        if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to begin recording command buffer");
        }
    }

    void VKTransferCommandBuffer::endRecording() {
        if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to end recording command buffer");
        }
    }

	void VKTransferCommandBuffer::copyBufferToBuffer(VKBuffer& source, const size_t sourceOffset, VKBuffer& destination, const size_t destinationOffset, const size_t sizeBytes) {
        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = sourceOffset;
        copyRegion.dstOffset = destinationOffset;
        copyRegion.size      = sizeBytes;
        vkCmdCopyBuffer(commandBuffer, source.getBuffer(), destination.getBuffer(), 1, &copyRegion);
    }

    void VKTransferCommandBuffer::transitionImageLayout(VKImage& image, ImageLayout previousLayout, ImageLayout newLayout) {
        VkImageMemoryBarrier barrier{};
        barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.pNext                           = nullptr;
        barrier.srcAccessMask                   = 0;//TODO
        barrier.dstAccessMask                   = 0;//TODO
        barrier.oldLayout                       = convertImageLayout(previousLayout);
        barrier.newLayout                       = convertImageLayout(newLayout);
        barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.image                           = image.getImage();
        barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;//TODO: Handle other aspect masks
        barrier.subresourceRange.baseMipLevel   = 0;
        barrier.subresourceRange.levelCount     = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = 1;
        vkCmdPipelineBarrier(commandBuffer, 0 /* TODO */, 0 /* TODO */, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    }

	VkCommandBuffer VKTransferCommandBuffer::getCommandBuffer() const {
        return commandBuffer;
    }
}