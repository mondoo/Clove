#pragma once

//TODO: Remove
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    ImageFormat convertImageFormat(VkFormat vulkanFormat);
    VkFormat convertImageFormat(ImageFormat garlicFormat);

    Result convertResult(VkResult result);

    VkFormat convertAttributeFormat(VertexAttributeFormat garlicFormat);

    VkDescriptorType getDescriptorType(DescriptorType garlicType);

    uint32_t getMemoryTypeIndex(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);
    VkMemoryPropertyFlags getMemoryPropertyFlags(MemoryProperties garlicProperties);

    VkImageLayout convertImageLayout(ImageLayout garlicFormat);

    VkAttachmentLoadOp convertLoadOp(LoadOperation garlicOperation);

    VkAttachmentStoreOp convertStoreOp(StoreOperation garlicOperation);

    VkPipelineStageFlags convertPipelineStage(PipelineStage garlicStage);

    VkAccessFlags convertAccessType(AccessType garlicAccess);
}