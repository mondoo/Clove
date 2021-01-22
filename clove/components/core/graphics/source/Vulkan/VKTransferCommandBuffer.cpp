#include "Clove/Graphics/Vulkan/VKTransferCommandBuffer.hpp"

#include "Clove/Graphics/Vulkan/VKBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKCommandBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKGraphicsResource.hpp"
#include "Clove/Graphics/Vulkan/VKImage.hpp"
#include "Clove/Graphics/Vulkan/VKMemoryBarrier.hpp"
#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"

#include <Clove/Cast.hpp>
#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    VKTransferCommandBuffer::VKTransferCommandBuffer(VkCommandBuffer commandBuffer, QueueFamilyIndices queueFamilyIndices)
        : commandBuffer(commandBuffer)
        , queueFamilyIndices(std::move(queueFamilyIndices)) {
    }

    VKTransferCommandBuffer::VKTransferCommandBuffer(VKTransferCommandBuffer &&other) noexcept = default;

    VKTransferCommandBuffer &VKTransferCommandBuffer::operator=(VKTransferCommandBuffer &&other) noexcept = default;

    VKTransferCommandBuffer::~VKTransferCommandBuffer() = default;

    void VKTransferCommandBuffer::beginRecording(CommandBufferUsage usageFlag) {
        VkCommandBufferBeginInfo beginInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags            = getCommandBufferUsageFlags(usageFlag),
            .pInheritanceInfo = nullptr,
        };

        if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to begin recording command buffer");
        }
    }

    void VKTransferCommandBuffer::endRecording() {
        if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to end recording command buffer");
        }
    }

    void VKTransferCommandBuffer::copyBufferToBuffer(GhaBuffer &source, size_t const sourceOffset, GhaBuffer &destination, size_t const destinationOffset, size_t const sizeBytes) {
        VkBufferCopy copyRegion{
            .srcOffset = sourceOffset,
            .dstOffset = destinationOffset,
            .size      = sizeBytes,
        };

        vkCmdCopyBuffer(commandBuffer, polyCast<VKBuffer>(&source)->getBuffer(), polyCast<VKBuffer>(&destination)->getBuffer(), 1, &copyRegion);
    }

    void VKTransferCommandBuffer::copyBufferToImage(GhaBuffer &source, size_t const sourceOffset, GhaImage &destination, vec3i const &destinationOffset, vec3ui const &destinationExtent) {
        VkBufferImageCopy copyRegion{
            .bufferOffset      = sourceOffset,
            .bufferRowLength   = 0,//Tightly packed
            .bufferImageHeight = 0,//Tightly packed
            .imageSubresource  = {
                .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,//TODO: Handle other aspect masks
                .mipLevel       = 0,
                .baseArrayLayer = 0,
                .layerCount     = 1,
            },
            .imageOffset = { destinationOffset.x, destinationOffset.y, destinationOffset.z },
            .imageExtent = { destinationExtent.x, destinationExtent.y, destinationExtent.z },
        };

        vkCmdCopyBufferToImage(commandBuffer, polyCast<VKBuffer>(&source)->getBuffer(), polyCast<VKImage>(&destination)->getImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
    }

    void VKTransferCommandBuffer::copyImageToBuffer(GhaImage &source, vec3i const &sourceOffset, vec3ui const &sourceExtent, GhaBuffer &destination, size_t const destinationOffset) {
        VkBufferImageCopy copyRegion{
            .bufferOffset      = destinationOffset,
            .bufferRowLength   = 0,//Tightly packed
            .bufferImageHeight = 0,//Tightly packed
            .imageSubresource  = {
                .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,//TODO: Handle other aspect masks
                .mipLevel       = 0,
                .baseArrayLayer = 0,
                .layerCount     = 1,
            },
            .imageOffset = { sourceOffset.x, sourceOffset.y, sourceOffset.z },
            .imageExtent = { sourceExtent.x, sourceExtent.y, sourceExtent.z },
        };

        vkCmdCopyImageToBuffer(commandBuffer, polyCast<VKImage>(&source)->getImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, polyCast<VKBuffer>(&destination)->getBuffer(), 1, &copyRegion);
    }

    void VKTransferCommandBuffer::bufferMemoryBarrier(GhaBuffer &buffer, BufferMemoryBarrierInfo const &barrierInfo, GhaPipelineObject::Stage sourceStage, GhaPipelineObject::Stage destinationStage) {
        uint32_t const sourceFamilyIndex{ getQueueFamilyIndex(barrierInfo.sourceQueue, queueFamilyIndices) };
        uint32_t const destinationFamilyIndex{ getQueueFamilyIndex(barrierInfo.destinationQueue, queueFamilyIndices) };

        VkBufferMemoryBarrier barrier{
            .sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
            .pNext               = nullptr,
            .srcAccessMask       = convertAccessFlags(barrierInfo.currentAccess),
            .dstAccessMask       = convertAccessFlags(barrierInfo.newAccess),
            .srcQueueFamilyIndex = sourceFamilyIndex,
            .dstQueueFamilyIndex = destinationFamilyIndex,
            .buffer              = polyCast<VKBuffer>(&buffer)->getBuffer(),
            .offset              = 0,
            .size                = VK_WHOLE_SIZE,
        };

        VkPipelineStageFlags const vkSourceStage{ VKPipelineObject::convertStage(sourceStage) };
        VkPipelineStageFlags const vkDestinationStage{ VKPipelineObject::convertStage(destinationStage) };

        vkCmdPipelineBarrier(commandBuffer, vkSourceStage, vkDestinationStage, 0, 0, nullptr, 1, &barrier, 0, nullptr);
    }

    void VKTransferCommandBuffer::imageMemoryBarrier(GhaImage &image, ImageMemoryBarrierInfo const &barrierInfo, GhaPipelineObject::Stage sourceStage, GhaPipelineObject::Stage destinationStage) {
        bool const isValidLayout =
            barrierInfo.newImageLayout != GhaImage::Layout::ShaderReadOnlyOptimal &&
            barrierInfo.newImageLayout != GhaImage::Layout::ColourAttachmentOptimal &&
            barrierInfo.newImageLayout != GhaImage::Layout::DepthStencilAttachmentOptimal &&
            barrierInfo.newImageLayout != GhaImage::Layout::DepthStencilReadOnlyOptimal;

        if(!isValidLayout) {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "{0}: Invalid newLayout. This command buffer cannot handle transfering images to the following layouts:\n\tImageLayout::ShaderReadOnlyOptimal\n\tImageLayout::ColourAttachmentOptimal\n\tImageLayout::DepthStencilAttachmentOptimal\n\tImageLayout::DepthStencilReadOnlyOptimal", CLOVE_FUNCTION_NAME);
            return;
        }

        uint32_t const sourceFamilyIndex{ getQueueFamilyIndex(barrierInfo.sourceQueue, queueFamilyIndices) };
        uint32_t const destinationFamilyIndex{ getQueueFamilyIndex(barrierInfo.destinationQueue, queueFamilyIndices) };

        VkImageMemoryBarrier barrier{
            .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext               = nullptr,
            .srcAccessMask       = convertAccessFlags(barrierInfo.currentAccess),
            .dstAccessMask       = convertAccessFlags(barrierInfo.newAccess),
            .oldLayout           = VKImage::convertLayout(barrierInfo.currentImageLayout),
            .newLayout           = VKImage::convertLayout(barrierInfo.newImageLayout),
            .srcQueueFamilyIndex = sourceFamilyIndex,
            .dstQueueFamilyIndex = destinationFamilyIndex,
            .image               = polyCast<VKImage>(&image)->getImage(),
            .subresourceRange    = {
                .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,//TODO: Handle other aspect masks
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = 1,
            },
        };

        VkPipelineStageFlags const vkSourceStage{ VKPipelineObject::convertStage(sourceStage) };
        VkPipelineStageFlags const vkDestinationStage{ VKPipelineObject::convertStage(destinationStage) };

        vkCmdPipelineBarrier(commandBuffer, vkSourceStage, vkDestinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    }

    VkCommandBuffer VKTransferCommandBuffer::getCommandBuffer() const {
        return commandBuffer;
    }
}