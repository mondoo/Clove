#include "Clove/Graphics/Vulkan/VulkanSwapchain.hpp"

#include "Clove/Graphics/Vulkan/VulkanImage.hpp"
#include "Clove/Graphics/Vulkan/VulkanResult.hpp"
#include "Clove/Graphics/Vulkan/VulkanSemaphore.hpp"

#include <Clove/Cast.hpp>

namespace clove {
    VulkanSwapchain::VulkanSwapchain(DevicePointer device, VkSwapchainKHR swapchain, VkFormat swapChainImageFormat, VkExtent2D swapChainExtent, std::vector<std::unique_ptr<VulkanImage>> images)
        : device{ std::move(device) }
        , swapchain{ swapchain }
        , swapChainImageFormat{ swapChainImageFormat }
        , swapChainExtent{ swapChainExtent }
        , images{ std::move(images) } {
    }

    VulkanSwapchain::VulkanSwapchain(VulkanSwapchain &&other) noexcept = default;

    VulkanSwapchain &VulkanSwapchain::operator=(VulkanSwapchain &&other) noexcept = default;

    VulkanSwapchain::~VulkanSwapchain() {
        vkDestroySwapchainKHR(device.get(), swapchain, nullptr);
    }

    std::pair<uint32_t, Result> VulkanSwapchain::aquireNextImage(GhaSemaphore const *availableSemaphore) {
        uint32_t outImageIndex{ 0 };
        VkSemaphore vkSemaphore{ availableSemaphore != nullptr ? polyCast<VulkanSemaphore const>(availableSemaphore)->getSemaphore() : VK_NULL_HANDLE };
        VkResult const result{ vkAcquireNextImageKHR(device.get(), swapchain, UINT64_MAX, vkSemaphore, VK_NULL_HANDLE, &outImageIndex) };

        return { outImageIndex, convertResult(result) };
    }

    GhaImage::Format VulkanSwapchain::getImageFormat() const {
        return VulkanImage::convertFormat(swapChainImageFormat);
    }

    vec2ui VulkanSwapchain::getSize() const {
        return { swapChainExtent.width, swapChainExtent.height };
    }

    std::vector<GhaImage *> VulkanSwapchain::getImages() const {
        std::vector<GhaImage *> ghaImages{};
        ghaImages.reserve(images.size());

        for(auto const &image : images){
            ghaImages.push_back(image.get());
        }

        return ghaImages;
    }

    VkSwapchainKHR VulkanSwapchain::getSwapchain() const {
        return swapchain;
    }
}