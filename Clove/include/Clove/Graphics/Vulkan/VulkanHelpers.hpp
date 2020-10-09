#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/Shader.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    VkImageView createImageView(VkDevice device, VkImage image, VkImageViewType viewType, VkFormat format, VkImageAspectFlags aspectFlags);

    VkShaderStageFlags convertShaderStage(Shader::Stage stage);

    Result convertResult(VkResult result);

    VkFormat convertAttributeFormat(VertexAttributeFormat garlicFormat);

    VkDescriptorType getDescriptorType(DescriptorType garlicType);

    uint32_t getQueueFamilyIndex(QueueType type, const QueueFamilyIndices& indices);

    VkCommandPoolCreateFlags convertCommandPoolCreateFlags(QueueFlags garlicFlags);

    VkCommandBufferUsageFlags getCommandBufferUsageFlags(CommandBufferUsage garlicUsage);
    VkIndexType getIndexType(IndexType garlicType);
}