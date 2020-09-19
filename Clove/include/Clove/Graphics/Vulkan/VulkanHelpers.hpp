#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    VkImageView createImageView(VkDevice device, VkImage image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectFlags);

    VkShaderStageFlags convertShaderStage(ShaderStage stage);

    ImageFormat convertImageFormat(VkFormat vulkanFormat);
    VkFormat convertImageFormat(ImageFormat garlicFormat);

    Result convertResult(VkResult result);

    VkFormat convertAttributeFormat(VertexAttributeFormat garlicFormat);

    VkDescriptorType getDescriptorType(DescriptorType garlicType);

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