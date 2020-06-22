#pragma once

#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"
#include "Clove/Graphics/Vulkan/VKImageView.hpp"
#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"

//TODO: Remove
#include <vulkan/vulkan.h>

namespace clv::gfx {
	struct SwapchainDescriptor {
		clv::mth::vec2ui extent;
	};
}

namespace clv::gfx::vk {
	class VKSwapchain {
		//VARIABLES
	private:
		VkDevice device = VK_NULL_HANDLE;
		VkSwapchainKHR swapChain = VK_NULL_HANDLE;

		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;

		std::vector<VkImage> images;
		std::vector<std::shared_ptr<VKImageView>> imageViews;

		//FUNCTIONS
	public:
		//TODO: Ctors
		//TODO: Put factory params into a struct
		VKSwapchain(VkDevice device, SwapchainSupportDetails supportDetails, VkSurfaceKHR surface, const QueueFamilyIndices& familyIndices, SwapchainDescriptor descriptor);
		~VKSwapchain();

		//TODO: a 'present' function, takes the presentation queue
		//TODO: a 'resize' function

		ImageFormat getImageFormat() const;
		VkExtent2D getExtent() const;

		uint32_t aquireNextImage(const VKSemaphore* semaphore);

		const VkSwapchainKHR& getSwapchain() const;

		const std::vector<std::shared_ptr<VKImageView>>& getImageViews() const;
	};
}