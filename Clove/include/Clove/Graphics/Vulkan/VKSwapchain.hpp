#pragma once

#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

//TODO: Remove
#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
	class SwapChainDescriptor {
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

		std::vector<VkImage> swapChainImages;
		std::vector<VkImageView> swapChainImageViews;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKSwapchain(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const QueueFamilyIndices& familyIndices, SwapChainDescriptor descriptor);
		~VKSwapchain();

		//TODO: a 'present' function, takes the presentation queue
		//TODO: a 'resize' function
	};
}