#pragma once

#include "Clove/Graphics/GhaSwapchain.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include <vulkan/vulkan.h>

namespace clove {
    class VulkanImage;
}

namespace clove {
    class VulkanSwapchain : public GhaSwapchain {
        //VARIABLES
    private:
        DevicePointer device;
        VkSwapchainKHR swapchain{ VK_NULL_HANDLE };

        VkFormat swapChainImageFormat{};
        VkExtent2D swapChainExtent{};

        std::vector<std::unique_ptr<VulkanImage>> images;

        //FUNCTIONS
    public:
        VulkanSwapchain() = delete;
        VulkanSwapchain(DevicePointer device, VkSwapchainKHR swapchain, VkFormat swapChainImageFormat, VkExtent2D swapChainExtent, std::vector<std::unique_ptr<VulkanImage>> images);

        VulkanSwapchain(VulkanSwapchain const &other) = delete;
        VulkanSwapchain(VulkanSwapchain &&other) noexcept;

        VulkanSwapchain &operator=(VulkanSwapchain const &other) = delete;
        VulkanSwapchain &operator=(VulkanSwapchain &&other) noexcept;

        ~VulkanSwapchain();

        std::pair<uint32_t, Result> aquireNextImage(GhaSemaphore const *availableSemaphore) override;
        
        GhaImage::Format getImageFormat() const override;
        vec2ui getSize() const override;

        std::vector<GhaImage *> getImages() const override;

        VkSwapchainKHR getSwapchain() const;
    };
}