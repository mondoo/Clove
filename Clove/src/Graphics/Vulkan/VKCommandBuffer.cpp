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
                GARLIC_ASSERT(false, "{0}: Unkown usage type", GARLIC_FUNCTION_NAME);
                return 0;
        }
    }

	static VkIndexType getIndexType(IndexType garlicType){
        switch(garlicType) {
            case IndexType::Uint16:
                return VK_INDEX_TYPE_UINT16;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown index type", GARLIC_FUNCTION_NAME);
                return VK_INDEX_TYPE_UINT16;
        }
	}

    static std::pair<VkAccessFlags, VkAccessFlags> getAccessFlags(VkImageLayout oldLayout, VkImageLayout newLayout){
        if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            return { 0, VK_ACCESS_TRANSFER_WRITE_BIT };
        } else if(oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            return { VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT };
        } else if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            return { 0, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT };
        } else {
            GARLIC_ASSERT(false, "{0}: Unhandled layout transition", GARLIC_FUNCTION_NAME);
            return { 0, 0 };
        }
    }

    static std::pair<VkPipelineStageFlags, VkPipelineStageFlags> getStageFlags(VkImageLayout oldLayout, VkImageLayout newLayout){
        if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            return { VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT };
        } else if(oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            return { VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT };
        } else if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            return { VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT };
        } else {
            GARLIC_ASSERT(false, "{0}: Unhandled layout transition", GARLIC_FUNCTION_NAME);
            return { 0, 0 };
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

	void VKGraphicsCommandBuffer::beginRenderPass(VKRenderPass& renderPass, VKFramebuffer& frameBuffer, const RenderArea& renderArea, const mth::vec4f& clearColour, const DepthStencilValue& depthStencilClearValue) {
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color    = { clearColour.r, clearColour.g, clearColour.b, clearColour.a };
        clearValues[1].depthStencil = { depthStencilClearValue.depth, depthStencilClearValue.stencil };

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType			 = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass		 = renderPass.getRenderPass();
		renderPassInfo.framebuffer		 = frameBuffer.getFrameBuffer();
		renderPassInfo.renderArea.offset = { renderArea.origin.x, renderArea.origin.y };
		renderPassInfo.renderArea.extent = { renderArea.size.x, renderArea.size.y };
		renderPassInfo.clearValueCount	 = std::size(clearValues);
		renderPassInfo.pClearValues		 = std::data(clearValues);

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
        const VkImageLayout vkPrevLayout = convertImageLayout(previousLayout);
        const VkImageLayout vkNextLayout = convertImageLayout(newLayout);

        //src = what happens before the barrier, dst = what needs to wait on the barrier
        //TODO: Take src and dst as params
        const auto [srcAccess, dstAccess] = getAccessFlags(vkPrevLayout, vkNextLayout);
        const auto [srcStage, dstStage]   = getStageFlags(vkPrevLayout, vkNextLayout);

        VkImageMemoryBarrier barrier{};
        barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.pNext                           = nullptr;
        barrier.srcAccessMask                   = srcAccess;
        barrier.dstAccessMask                   = dstAccess;
        barrier.oldLayout                       = vkPrevLayout;
        barrier.newLayout                       = vkNextLayout;
        barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.image                           = image.getImage();
        barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;//TODO: Handle other aspect masks
        barrier.subresourceRange.baseMipLevel   = 0;
        barrier.subresourceRange.levelCount     = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = 1;
        vkCmdPipelineBarrier(commandBuffer, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
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

    void VKTransferCommandBuffer::copyBufferToImage(VKBuffer& source, const size_t sourceOffset, VKImage& destination, ImageLayout destinationLayout, const mth::vec3i& destinationOffset, const mth::vec3ui& destinationExtent) {
        VkBufferImageCopy copyRegion{};
        copyRegion.bufferOffset                    = sourceOffset;
        copyRegion.bufferRowLength                 = 0;                        //Tightly packed
        copyRegion.bufferImageHeight               = 0;                        //Tightly packed
        copyRegion.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;//TODO: Handle other aspect masks
        copyRegion.imageSubresource.mipLevel       = 0;
        copyRegion.imageSubresource.baseArrayLayer = 0;
        copyRegion.imageSubresource.layerCount     = 1;
        copyRegion.imageOffset                     = { destinationOffset.x, destinationOffset.y, destinationOffset.z };
        copyRegion.imageExtent                     = { destinationExtent.x, destinationExtent.y, destinationExtent.z };
        vkCmdCopyBufferToImage(commandBuffer, source.getBuffer(), destination.getImage(), convertImageLayout(destinationLayout), 1, &copyRegion);
    }

    void VKTransferCommandBuffer::transitionImageLayout(VKImage& image, ImageLayout previousLayout, ImageLayout newLayout) {
        const bool isValidLayout =
            newLayout != ImageLayout::ShaderReadOnlyOptimal &&
            newLayout != ImageLayout::ColourAttachmentOptimal &&
            newLayout != ImageLayout::DepthStencilAttachmentOptimal &&
            newLayout != ImageLayout::DepthStencilReadOnlyOptimal;

        if(!isValidLayout) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "{0}: Invalid newLayout. This command buffer cannot handle transfering images to the following layouts:\n\tImageLayout::ShaderReadOnlyOptimal\n\tImageLayout::ColourAttachmentOptimal\n\tImageLayout::DepthStencilAttachmentOptimal\n\tImageLayout::DepthStencilReadOnlyOptimal", GARLIC_FUNCTION_NAME);
            return;
        }

        const VkImageLayout vkPrevLayout = convertImageLayout(previousLayout);
        const VkImageLayout vkNextLayout = convertImageLayout(newLayout);

        //src = what happens before the barrier, dst = what needs to wait on the barrier
        //TODO: Take src and dst as params
        const auto [srcAccess, dstAccess] = getAccessFlags(vkPrevLayout, vkNextLayout);
        const auto [srcStage, dstStage]   = getStageFlags(vkPrevLayout, vkNextLayout);

        VkImageMemoryBarrier barrier{};
        barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.pNext                           = nullptr;
        barrier.srcAccessMask                   = srcAccess;
        barrier.dstAccessMask                   = dstAccess;
        barrier.oldLayout                       = vkPrevLayout;
        barrier.newLayout                       = vkNextLayout;
        barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.image                           = image.getImage();
        barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;//TODO: Handle other aspect masks
        barrier.subresourceRange.baseMipLevel   = 0;
        barrier.subresourceRange.levelCount     = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = 1;
        vkCmdPipelineBarrier(commandBuffer, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    }

	VkCommandBuffer VKTransferCommandBuffer::getCommandBuffer() const {
        return commandBuffer;
    }
}