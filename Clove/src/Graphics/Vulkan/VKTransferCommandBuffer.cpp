#include "Clove/Graphics/Vulkan/VKTransferCommandBuffer.hpp"

#include "Clove/Graphics/Vulkan/VKBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKImage.hpp"
#include "Clove/Graphics/Vulkan/VulkanHelpers.hpp"
#include "Clove/Log.hpp"
#include "Clove/Utils/Cast.hpp"

#include <Root/Definitions.hpp>

namespace clv::gfx::vk {
    VKTransferCommandBuffer::VKTransferCommandBuffer(VkCommandBuffer commandBuffer, QueueFamilyIndices queueFamilyIndices)
        : commandBuffer(commandBuffer)
        , queueFamilyIndices(std::move(queueFamilyIndices)) {
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

    void VKTransferCommandBuffer::copyBufferToBuffer(GraphicsBuffer& source, const size_t sourceOffset, GraphicsBuffer& destination, const size_t destinationOffset, const size_t sizeBytes) {
        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = sourceOffset;
        copyRegion.dstOffset = destinationOffset;
        copyRegion.size      = sizeBytes;

        vkCmdCopyBuffer(commandBuffer, polyCast<VKBuffer>(&source)->getBuffer(), polyCast<VKBuffer>(&destination)->getBuffer(), 1, &copyRegion);
    }

    void VKTransferCommandBuffer::copyBufferToImage(GraphicsBuffer& source, const size_t sourceOffset, GraphicsImage& destination, ImageLayout destinationLayout, const mth::vec3i& destinationOffset, const mth::vec3ui& destinationExtent) {
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

        vkCmdCopyBufferToImage(commandBuffer, polyCast<VKBuffer>(&source)->getBuffer(), polyCast<VKImage>(&destination)->getImage(), convertImageLayout(destinationLayout), 1, &copyRegion);
    }

    void VKTransferCommandBuffer::bufferMemoryBarrier(GraphicsBuffer& buffer, const BufferMemoryBarrierInfo& barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage) {
        const uint32_t sourceFamilyIndex      = getQueueFamilyIndex(barrierInfo.sourceQueue, queueFamilyIndices);
        const uint32_t destinationFamilyIndex = getQueueFamilyIndex(barrierInfo.destinationQueue, queueFamilyIndices);

        VkBufferMemoryBarrier barrier{};
        barrier.sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
        barrier.pNext               = nullptr;
        barrier.srcAccessMask       = convertAccessFlags(barrierInfo.sourceAccess);
        barrier.dstAccessMask       = convertAccessFlags(barrierInfo.destinationAccess);
        barrier.srcQueueFamilyIndex = sourceFamilyIndex;
        barrier.dstQueueFamilyIndex = destinationFamilyIndex;
        barrier.buffer              = polyCast<VKBuffer>(&buffer)->getBuffer();
        barrier.offset              = 0;
        barrier.size                = VK_WHOLE_SIZE;

        const VkPipelineStageFlags vkSourceStage      = convertPipelineStage(sourceStage);
        const VkPipelineStageFlags vkDestinationStage = convertPipelineStage(destinationStage);

        vkCmdPipelineBarrier(commandBuffer, vkSourceStage, vkDestinationStage, 0, 0, nullptr, 1, &barrier, 0, nullptr);
    }

    void VKTransferCommandBuffer::imageMemoryBarrier(GraphicsImage& image, const ImageMemoryBarrierInfo& barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage) {
        const bool isValidLayout =
            barrierInfo.newImageLayout != ImageLayout::ShaderReadOnlyOptimal &&
            barrierInfo.newImageLayout != ImageLayout::ColourAttachmentOptimal &&
            barrierInfo.newImageLayout != ImageLayout::DepthStencilAttachmentOptimal &&
            barrierInfo.newImageLayout != ImageLayout::DepthStencilReadOnlyOptimal;

        if(!isValidLayout) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "{0}: Invalid newLayout. This command buffer cannot handle transfering images to the following layouts:\n\tImageLayout::ShaderReadOnlyOptimal\n\tImageLayout::ColourAttachmentOptimal\n\tImageLayout::DepthStencilAttachmentOptimal\n\tImageLayout::DepthStencilReadOnlyOptimal", GARLIC_FUNCTION_NAME);
            return;
        }
    
        const uint32_t sourceFamilyIndex      = getQueueFamilyIndex(barrierInfo.sourceQueue, queueFamilyIndices);
        const uint32_t destinationFamilyIndex = getQueueFamilyIndex(barrierInfo.destinationQueue, queueFamilyIndices);

        VkImageMemoryBarrier barrier{};
        barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.pNext                           = nullptr;
        barrier.srcAccessMask                   = convertAccessFlags(barrierInfo.sourceAccess);
        barrier.dstAccessMask                   = convertAccessFlags(barrierInfo.destinationAccess);
        barrier.oldLayout                       = convertImageLayout(barrierInfo.oldImageLayout);
        barrier.newLayout                       = convertImageLayout(barrierInfo.newImageLayout);
        barrier.srcQueueFamilyIndex             = sourceFamilyIndex;
        barrier.dstQueueFamilyIndex             = destinationFamilyIndex;
        barrier.image                           = polyCast<VKImage>(&image)->getImage();
        barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;//TODO: Handle other aspect masks
        barrier.subresourceRange.baseMipLevel   = 0;
        barrier.subresourceRange.levelCount     = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = 1;

        const VkPipelineStageFlags vkSourceStage      = convertPipelineStage(sourceStage);
        const VkPipelineStageFlags vkDestinationStage = convertPipelineStage(destinationStage);

        vkCmdPipelineBarrier(commandBuffer, vkSourceStage, vkDestinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    }

    VkCommandBuffer VKTransferCommandBuffer::getCommandBuffer() const {
        return commandBuffer;
    }
}