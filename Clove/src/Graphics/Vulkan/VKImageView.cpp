#include "Clove/Graphics/Vulkan/VKImageView.hpp"

namespace clv::gfx::vk {
	VKImageView::VKImageView(VkDevice device, VkImageView imageView)
		: device(device)
		, imageView(imageView) {
	}

	VKImageView::~VKImageView() {
		vkDestroyImageView(device, imageView, nullptr);
	}

	VkImageView VKImageView::getImageView() const {
		return imageView;
	}
}