#include "Clove/Graphics/Vulkan/VulkanComputeCommandBuffer.hpp"

#include "Clove/Graphics/Vulkan/VulkanBuffer.hpp"
#include "Clove/Graphics/Vulkan/VulkanCommandBuffer.hpp"
#include "Clove/Graphics/Vulkan/VulkanImage.hpp"
#include "Clove/Graphics/Vulkan/VulkanMemoryBarrier.hpp"
#include "Clove/Graphics/Vulkan/VulkanPipelineObject.hpp"
#include "Clove/Graphics/Vulkan/VulkanResource.hpp"
#include "Clove/Graphics/Vulkan/VulkanDescriptorSet.hpp"
#include "Clove/Graphics/Vulkan/VulkanComputePipelineObject.hpp"

#include <Clove/Cast.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    VulkanComputeCommandBuffer::VulkanComputeCommandBuffer(VkCommandBuffer commandBuffer, QueueFamilyIndices queueFamilyIndices)
        : commandBuffer{ commandBuffer }
        , queueFamilyIndices{ std::move(queueFamilyIndices) } {
    }

    VulkanComputeCommandBuffer::VulkanComputeCommandBuffer(VulkanComputeCommandBuffer &&other) noexcept = default;

    VulkanComputeCommandBuffer &VulkanComputeCommandBuffer::operator=(VulkanComputeCommandBuffer &&other) noexcept = default;

    VulkanComputeCommandBuffer::~VulkanComputeCommandBuffer() = default;

    void VulkanComputeCommandBuffer::beginRecording(CommandBufferUsage usageFlag) {
        VkCommandBufferBeginInfo beginInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags            = getCommandBufferUsageFlags(usageFlag),
            .pInheritanceInfo = nullptr,
        };

        if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to begin recording command buffer");
        }
    }

    void VulkanComputeCommandBuffer::endRecording() {
        if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to end recording command buffer");
        }
    }

    void VulkanComputeCommandBuffer::bindPipelineObject(GhaComputePipelineObject &pipelineObject) {
        auto const *pipeline{ polyCast<VulkanComputePipelineObject>(&pipelineObject) };

        currentLayout = pipeline->getLayout();

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline->getPipeline());
    }

    void VulkanComputeCommandBuffer::bindDescriptorSet(GhaDescriptorSet &descriptorSet, uint32_t const setNum) {
        VkDescriptorSet sets[] = { polyCast<VulkanDescriptorSet>(&descriptorSet)->getDescriptorSet() };

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, currentLayout, setNum, 1, sets, 0, nullptr);
    }

    void VulkanComputeCommandBuffer::pushConstant(size_t const offset, size_t const size, void const *data) {
        vkCmdPushConstants(commandBuffer, currentLayout, VK_SHADER_STAGE_COMPUTE_BIT, offset, size, data);
    }

    void VulkanComputeCommandBuffer::disptach(vec3ui groupCount) {
        vkCmdDispatch(commandBuffer, groupCount.x, groupCount.y, groupCount.z);
    }

    void VulkanComputeCommandBuffer::bufferMemoryBarrier(GhaBuffer &buffer, BufferMemoryBarrierInfo const &barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage) {
        const uint32_t sourceFamilyIndex{ getQueueFamilyIndex(barrierInfo.sourceQueue, queueFamilyIndices) };
        const uint32_t destinationFamilyIndex{ getQueueFamilyIndex(barrierInfo.destinationQueue, queueFamilyIndices) };

        VkBufferMemoryBarrier barrier{
            .sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
            .pNext               = nullptr,
            .srcAccessMask       = convertAccessFlags(barrierInfo.currentAccess),
            .dstAccessMask       = convertAccessFlags(barrierInfo.newAccess),
            .srcQueueFamilyIndex = sourceFamilyIndex,
            .dstQueueFamilyIndex = destinationFamilyIndex,
            .buffer              = polyCast<VulkanBuffer>(&buffer)->getBuffer(),
            .offset              = 0,
            .size                = VK_WHOLE_SIZE,
        };

        VkPipelineStageFlags const vkSourceStage{ convertStage(sourceStage) };
        VkPipelineStageFlags const vkDestinationStage{ convertStage(destinationStage) };

        vkCmdPipelineBarrier(commandBuffer, vkSourceStage, vkDestinationStage, 0, 0, nullptr, 1, &barrier, 0, nullptr);
    }

    void VulkanComputeCommandBuffer::imageMemoryBarrier(GhaImage &image, ImageMemoryBarrierInfo const &barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage) {
        uint32_t const sourceFamilyIndex{ getQueueFamilyIndex(barrierInfo.sourceQueue, queueFamilyIndices) };
        uint32_t const destinationFamilyIndex{ getQueueFamilyIndex(barrierInfo.destinationQueue, queueFamilyIndices) };

        VkImageAspectFlags const aspectFlags{ static_cast<VkImageAspectFlags>(image.getDescriptor().format == GhaImage::Format::D32_SFLOAT ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT) };

        VkImageMemoryBarrier const barrier{
            .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext               = nullptr,
            .srcAccessMask       = convertAccessFlags(barrierInfo.currentAccess),
            .dstAccessMask       = convertAccessFlags(barrierInfo.newAccess),
            .oldLayout           = VulkanImage::convertLayout(barrierInfo.currentImageLayout),
            .newLayout           = VulkanImage::convertLayout(barrierInfo.newImageLayout),
            .srcQueueFamilyIndex = sourceFamilyIndex,
            .dstQueueFamilyIndex = destinationFamilyIndex,
            .image               = polyCast<VulkanImage>(&image)->getImage(),
            .subresourceRange    = {
                .aspectMask     = aspectFlags,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = 1,
            },
        };

        VkPipelineStageFlags const vkSourceStage{ convertStage(sourceStage) };
        VkPipelineStageFlags const vkDestinationStage{ convertStage(destinationStage) };

        vkCmdPipelineBarrier(commandBuffer, vkSourceStage, vkDestinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    }

    VkCommandBuffer VulkanComputeCommandBuffer::getCommandBuffer() const {
        return commandBuffer;
    }
}