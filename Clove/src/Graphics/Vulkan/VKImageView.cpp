#include "Clove/Graphics/Vulkan/VKImageView.hpp"

namespace clv::gfx::vk {
	VKImageView::VKImageView(VkDevice device, VkImageView imageView)
		: device(device)
		, imageView(imageView) {
    }

    VKImageView::VKImageView(VKImageView&& other) noexcept = default;

	VKImageView& VKImageView::operator=(VKImageView&& other) noexcept = default;

    VKImageView::~VKImageView() {
		vkDestroyImageView(device, imageView, nullptr);
	}

	VkImageView VKImageView::getImageView() const {
		return imageView;
	}
}