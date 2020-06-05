#include "Clove/Graphics/Vulkan/VKSwapChain.hpp"

namespace clv::gfx::vk {
	VKSwapchain::VKSwapchain(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const QueueFamilyIndices& familyIndices, SwapChainDescriptor descriptor) 
		: device(device){
		SwapChainSupportDetails swapChainSuport = querySwapChainSupport(physicalDevice);

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSuport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSuport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSuport.capabilities);

		uint32_t queueFamilyIndices[] = { *familyIndices.graphicsFamily, *familyIndices.presentFamily };

		//Request one more than the minimum images the swap chain can support because sometimes we might need to wait for the driver
		uint32_t imageCount = swapChainSuport.capabilities.minImageCount + 1;
		if(swapChainSuport.capabilities.maxImageCount > 0 && imageCount > swapChainSuport.capabilities.maxImageCount) {
			imageCount = swapChainSuport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		if(familyIndices.graphicsFamily != familyIndices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		} else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}
		createInfo.preTransform = swapChainSuport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if(vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
			throw std::runtime_error("failed to create swap chain!");
		}

		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

		swapChainImageFormat = surfaceFormat.format;
		swapChainExtent = extent;

		//TODO: Create image views
	}

	VKSwapchain::~VKSwapchain(){
		for(auto imageView : swapChainImageViews) {
			vkDestroyImageView(device, imageView, nullptr);
		}
		vkDestroySwapchainKHR(device, swapChain, nullptr);
	}
}