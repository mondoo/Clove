#include "Clove/Graphics/Vulkan/VKSwapChain.hpp"

#include "Clove/Graphics/Vulkan/VulkanHelpers.hpp"

namespace clv::gfx::vk {
	static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		for(const auto& availableFormat : availableFormats) {
			if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		//Fall back to the first one if we can't find a surface format we want
		return availableFormats[0];
	}

	static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
		for(const auto& availablePresentMode : availablePresentModes) {
			if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const VkExtent2D& windowExtent) {
		if(capabilities.currentExtent.width != UINT32_MAX) {//If width / height are max then the window manager is allowing us to differ in size
			return capabilities.currentExtent;
		} else {
			VkExtent2D actualExent;

			actualExent.width  = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, windowExtent.width));
			actualExent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, windowExtent.height));

			return actualExent;
		}
	}

	VKSwapchain::VKSwapchain(VkDevice device, SwapchainSupportDetails supportDetails, VkSurfaceKHR surface, const QueueFamilyIndices& familyIndices, SwapchainDescriptor descriptor)
		: device(device) {
		VkExtent2D windowExtent{
			descriptor.extent.x,
			descriptor.extent.y
		};

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(supportDetails.formats);
		VkPresentModeKHR presentMode	 = chooseSwapPresentMode(supportDetails.presentModes);
		VkExtent2D extent				 = chooseSwapExtent(supportDetails.capabilities, windowExtent);

		std::array queueFamilyIndices{ *familyIndices.graphicsFamily, *familyIndices.presentFamily };

		//Request one more than the minimum images the swap chain can support because sometimes we might need to wait for the driver
		uint32_t imageCount = supportDetails.capabilities.minImageCount + 1;
		if(supportDetails.capabilities.maxImageCount > 0 && imageCount > supportDetails.capabilities.maxImageCount) {
			imageCount = supportDetails.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType			= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface			= surface;
		createInfo.minImageCount	= imageCount;
		createInfo.imageFormat		= surfaceFormat.format;
		createInfo.imageColorSpace	= surfaceFormat.colorSpace;
		createInfo.imageExtent		= extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage		= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		if(familyIndices.graphicsFamily != familyIndices.presentFamily) {
			createInfo.imageSharingMode		 = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = std::size(queueFamilyIndices);
			createInfo.pQueueFamilyIndices	 = std::data(queueFamilyIndices);
		} else {
			createInfo.imageSharingMode		 = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices	 = nullptr;
		}
		createInfo.preTransform	  = supportDetails.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode	  = presentMode;
		createInfo.clipped		  = VK_TRUE;
		createInfo.oldSwapchain	  = VK_NULL_HANDLE;

		if(vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create swap chain");
			return;
		}

		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
		images.resize(imageCount);
		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, images.data());

		swapChainImageFormat = surfaceFormat.format;
		swapChainExtent		 = extent;

		imageViews.resize(std::size(images));
		for(size_t i = 0; i < images.size(); ++i) {
			imageViews[i] = std::make_shared<VKImageView>(device, createImageView(device, images[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT));
		}
	}

	VKSwapchain::~VKSwapchain() {
		vkDestroySwapchainKHR(device, swapChain, nullptr);
	}

	ImageFormat VKSwapchain::getImageFormat() const {
		return convertImageFormat(swapChainImageFormat);
	}

	VkExtent2D VKSwapchain::getExtent() const {
		return swapChainExtent;
	}

	Result VKSwapchain::aquireNextImage(const VKSemaphore* semaphore, uint32_t& outImageIndex) {
		VkSemaphore vkSemaphore = semaphore ? semaphore->getSemaphore() : VK_NULL_HANDLE;
		const VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, vkSemaphore, VK_NULL_HANDLE, &outImageIndex);

		return convertResult(result);
	}

	const VkSwapchainKHR& VKSwapchain::getSwapchain() const {
		return swapChain;
	}

	const std::vector<std::shared_ptr<VKImageView>>& VKSwapchain::getImageViews() const {
		return imageViews;
	}
}