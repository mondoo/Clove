#pragma once

//TODO: Remove
#include <vulkan/vulkan.h>

#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

namespace clv::gfx::vk {
	VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

	ImageFormat convertImageFormat(VkFormat vulkanFormat);
	VkFormat convertImageFormat(ImageFormat garlicFormat);

	Result convertResult(VkResult result);

	VkFormat convertAttributeFormat(VertexAttributeFormat garlicFormat);
}