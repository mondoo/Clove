#include "Clove/Graphics/Vulkan/VKTransferCommandBuffer.hpp"

#include "Clove/Graphics/Vulkan/VKBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKCommandBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKGraphicsResource.hpp"
#include "Clove/Graphics/Vulkan/VKImage.hpp"
#include "Clove/Graphics/Vulkan/VKMemoryBarrier.hpp"
#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"
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
        VkCommandBufferBeginInfo beginInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags            = getCommandBufferUsageFlags(usageFlag),
            .pInheritanceInfo = nullptr,
        };

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
        VkBufferCopy copyRegion{
            .srcOffset = sourceOffset,
            .dstOffset = destinationOffset,
            .size      = sizeBytes,
        };

        vkCmdCopyBuffer(commandBuffer, polyCast<VKBuffer>(&source)->getBuffer(), polyCast<VKBuffer>(&destination)->getBuffer(), 1, &copyRegion);
    }

    void VKTransferCommandBuffer::copyBufferToImage(GraphicsBuffer& source, const size_t sourceOffset, GraphicsImage& destination, GraphicsImage::Layout destinationLayout, const mth::vec3i& destinationOffset, const mth::vec3ui& destinationExtent) {
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

        vkCmdCopyBufferToImage(commandBuffer, polyCast<VKBuffer>(&source)->getBuffer(), polyCast<VKImage>(&destination)->getImage(), VKImage::convertLayout(destinationLayout), 1, &copyRegion);
    }

    void VKTransferCommandBuffer::bufferMemoryBarrier(GraphicsBuffer& buffer, const BufferMemoryBarrierInfo& barrierInfo, PipelineObject::Stage sourceStage, PipelineObject::Stage destinationStage) {
        const uint32_t sourceFamilyIndex      = getQueueFamilyIndex(barrierInfo.sourceQueue, queueFamilyIndices);
        const uint32_t destinationFamilyIndex = getQueueFamilyIndex(barrierInfo.destinationQueue, queueFamilyIndices);

        VkBufferMemoryBarrier barrier{
            .sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
            .pNext               = nullptr,
            .srcAccessMask       = convertAccessFlags(barrierInfo.sourceAccess),
            .dstAccessMask       = convertAccessFlags(barrierInfo.destinationAccess),
            .srcQueueFamilyIndex = sourceFamilyIndex,
            .dstQueueFamilyIndex = destinationFamilyIndex,
            .buffer              = polyCast<VKBuffer>(&buffer)->getBuffer(),
            .offset              = 0,
            .size                = VK_WHOLE_SIZE,
        };

        const VkPipelineStageFlags vkSourceStage      = VKPipelineObject::convertStage(sourceStage);
        const VkPipelineStageFlags vkDestinationStage = VKPipelineObject::convertStage(destinationStage);

        vkCmdPipelineBarrier(commandBuffer, vkSourceStage, vkDestinationStage, 0, 0, nullptr, 1, &barrier, 0, nullptr);
    }

    void VKTransferCommandBuffer::imageMemoryBarrier(GraphicsImage& image, const ImageMemoryBarrierInfo& barrierInfo, PipelineObject::Stage sourceStage, PipelineObject::Stage destinationStage) {
        const bool isValidLayout =
            barrierInfo.newImageLayout != GraphicsImage::Layout::ShaderReadOnlyOptimal &&
            barrierInfo.newImageLayout != GraphicsImage::Layout::ColourAttachmentOptimal &&
            barrierInfo.newImageLayout != GraphicsImage::Layout::DepthStencilAttachmentOptimal &&
            barrierInfo.newImageLayout != GraphicsImage::Layout::DepthStencilReadOnlyOptimal;

        if(!isValidLayout) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "{0}: Invalid newLayout. This command buffer cannot handle transfering images to the following layouts:\n\tImageLayout::ShaderReadOnlyOptimal\n\tImageLayout::ColourAttachmentOptimal\n\tImageLayout::DepthStencilAttachmentOptimal\n\tImageLayout::DepthStencilReadOnlyOptimal", GARLIC_FUNCTION_NAME);
            return;
        }

        const uint32_t sourceFamilyIndex      = getQueueFamilyIndex(barrierInfo.sourceQueue, queueFamilyIndices);
        const uint32_t destinationFamilyIndex = getQueueFamilyIndex(barrierInfo.destinationQueue, queueFamilyIndices);

        VkImageMemoryBarrier barrier{
            .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext               = nullptr,
            .srcAccessMask       = convertAccessFlags(barrierInfo.sourceAccess),
            .dstAccessMask       = convertAccessFlags(barrierInfo.destinationAccess),
            .oldLayout           = VKImage::convertLayout(barrierInfo.oldImageLayout),
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

        const VkPipelineStageFlags vkSourceStage      = VKPipelineObject::convertStage(sourceStage);
        const VkPipelineStageFlags vkDestinationStage = VKPipelineObject::convertStage(destinationStage);

        vkCmdPipelineBarrier(commandBuffer, vkSourceStage, vkDestinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    }

    VkCommandBuffer VKTransferCommandBuffer::getCommandBuffer() const {
        return commandBuffer;
    }
}