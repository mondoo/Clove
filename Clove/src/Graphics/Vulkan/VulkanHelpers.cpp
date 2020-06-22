#include "Clove/Graphics/Vulkan/VulkanHelpers.hpp"

namespace clv::gfx::vk {
	VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType							 = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image							 = image;
		viewInfo.viewType						 = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format							 = format;
		viewInfo.subresourceRange.aspectMask	 = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel	 = 0;
		viewInfo.subresourceRange.levelCount	 = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount	 = 1;

		VkImageView imageView;
		if(vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create texture image view");
			return VK_NULL_HANDLE;
		}

		return imageView;
	}

	ImageFormat convertImageFormat(VkFormat vulkanFormat) {
		switch(vulkanFormat) {
			//Formats supported by garlic
			case VK_FORMAT_B8G8R8A8_SRGB:
				return ImageFormat::B8G8R8A8_SRGB;
			case VK_FORMAT_B8G8R8A8_UNORM:
				return ImageFormat::B8G8R8A8_UNORM;
			default:
				GARLIC_LOG(garlicLogContext, Log::Level::Error, "Format not supported by garlic");
				return ImageFormat::Unkown;
		}
	}

	VkFormat convertImageFormat(ImageFormat garlicFormat) {
		switch(garlicFormat) {
			case ImageFormat::Unkown:
				return VK_FORMAT_UNDEFINED;
			case ImageFormat::B8G8R8A8_SRGB:
				return VK_FORMAT_B8G8R8A8_SRGB;
			case ImageFormat::B8G8R8A8_UNORM:
				return VK_FORMAT_B8G8R8A8_UNORM;
		}
	}

	Result convertResult(VkResult result) {
		switch(result) {
			case VK_SUCCESS:
				return Result::Success;
			case VK_SUBOPTIMAL_KHR:
				return Result::Success_SwapchainSuboptimal;
			case VK_ERROR_OUT_OF_DATE_KHR:
				return Result::Error_SwapchainOutOfDate;
			default:
				CLV_ASSERT(false, "Unkown result type");
				return Result::Unkown;
		}
	}
}