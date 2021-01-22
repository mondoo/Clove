#include "Clove/Graphics/Vulkan/VKSwapchain.hpp"

#include "Clove/Graphics/Vulkan/VKImage.hpp"
#include "Clove/Graphics/Vulkan/VKImageView.hpp"
#include "Clove/Graphics/Vulkan/VKResult.hpp"
#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"

#include <Clove/Cast.hpp>

namespace garlic::clove {
    VKSwapchain::VKSwapchain(DevicePointer device, VkSwapchainKHR swapchain, VkFormat swapChainImageFormat, VkExtent2D swapChainExtent)
        : device{ std::move(device) }
        , swapchain{ swapchain }
        , swapChainImageFormat{ swapChainImageFormat }
        , swapChainExtent{ swapChainExtent } {
        uint32_t imageCount;
        vkGetSwapchainImagesKHR(this->device.get(), swapchain, &imageCount, nullptr);
        images.resize(imageCount);
        vkGetSwapchainImagesKHR(this->device.get(), swapchain, &imageCount, images.data());

        imageViews.resize(std::size(images));
        for(size_t i = 0; i < images.size(); ++i) {
            imageViews[i] = std::make_shared<VKImageView>(this->device.get(), VKImageView::create(this->device.get(), images[i], VK_IMAGE_VIEW_TYPE_2D, swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 0, 1));
        }
    }

    VKSwapchain::VKSwapchain(VKSwapchain &&other) noexcept = default;

    VKSwapchain &VKSwapchain::operator=(VKSwapchain &&other) noexcept = default;

    VKSwapchain::~VKSwapchain() {
        vkDestroySwapchainKHR(device.get(), swapchain, nullptr);
    }

    std::pair<uint32_t, Result> VKSwapchain::aquireNextImage(GhaSemaphore const *availableSemaphore) {
        uint32_t outImageIndex{ 0 };
        VkSemaphore vkSemaphore{ availableSemaphore ? polyCast<const VKSemaphore>(availableSemaphore)->getSemaphore() : VK_NULL_HANDLE };
        VkResult const result{ vkAcquireNextImageKHR(device.get(), swapchain, UINT64_MAX, vkSemaphore, VK_NULL_HANDLE, &outImageIndex) };

        return { outImageIndex, convertResult(result) };
    }

    GhaImage::Format VKSwapchain::getImageFormat() const {
        return VKImage::convertFormat(swapChainImageFormat);
    }

    vec2ui VKSwapchain::getSize() const {
        return { swapChainExtent.width, swapChainExtent.height };
    }

    std::vector<std::shared_ptr<GhaImageView>> VKSwapchain::getImageViews() const {
        return { imageViews.begin(), imageViews.end() };
    }

    VkSwapchainKHR VKSwapchain::getSwapchain() const {
        return swapchain;
    }
}