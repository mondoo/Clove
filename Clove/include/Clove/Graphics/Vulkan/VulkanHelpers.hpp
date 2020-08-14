#pragma once

#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"
#include "Clove/Graphics/GraphicsTypes.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    VkImageView createImageView(VkDevice device, VkImage image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectFlags);

    ImageFormat convertImageFormat(VkFormat vulkanFormat);
    VkFormat convertImageFormat(ImageFormat garlicFormat);

    Result convertResult(VkResult result);

    VkFormat convertAttributeFormat(VertexAttributeFormat garlicFormat);

    VkDescriptorType getDescriptorType(DescriptorType garlicType);

    uint32_t getMemoryTypeIndex(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);
    VkMemoryPropertyFlags getMemoryPropertyFlags(MemoryType garlicProperties);

    VkImageLayout convertImageLayout(ImageLayout garlicFormat);

    VkAttachmentLoadOp convertLoadOp(LoadOperation garlicOperation);

    VkAttachmentStoreOp convertStoreOp(StoreOperation garlicOperation);

    VkPipelineStageFlags convertPipelineStage(PipelineStage garlicStage);

    VkAccessFlags convertAccessFlags(AccessFlags garlicAccess);

    uint32_t getQueueFamilyIndex(QueueType type, const QueueFamilyIndices& indices);

    VkCommandPoolCreateFlags convertCommandPoolCreateFlags(QueueFlags garlicFlags);

    VkCommandBufferUsageFlags getCommandBufferUsageFlags(CommandBufferUsage garlicUsage);
    VkIndexType getIndexType(IndexType garlicType);
}