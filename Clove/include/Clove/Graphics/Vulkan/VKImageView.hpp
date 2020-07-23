#pragma once

//TODO: Remove
#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
	class VKImageView {
		//VARIABLES
	private:
		VkDevice device = VK_NULL_HANDLE;

		VkImageView imageView = VK_NULL_HANDLE;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKImageView(VkDevice device, VkImageView imageView);
		~VKImageView();

		//VK specific
		VkImageView getImageView() const;
	};
}