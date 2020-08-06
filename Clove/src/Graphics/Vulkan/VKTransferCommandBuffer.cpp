#include "Clove/Graphics/Vulkan/VKTransferCommandBuffer.hpp"

#include "Clove/Graphics/Vulkan/VulkanHelpers.hpp"

namespace clv::gfx::vk {
    VKTransferCommandBuffer::VKTransferCommandBuffer(VkCommandBuffer commandBuffer)
        : commandBuffer(commandBuffer) {
    }

    VKTransferCommandBuffer::VKTransferCommandBuffer(VKTransferCommandBuffer&& other) noexcept = default;

    VKTransferCommandBuffer& VKTransferCommandBuffer::operator=(VKTransferCommandBuffer&& other) noexcept = default;

    VKTransferCommandBuffer::~VKTransferCommandBuffer() = default;

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