#pragma once

#include "Clove/Graphics/GhaSwapchain.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    class VKImageView;
}

namespace garlic::clove {
    class VKSwapchain : public GhaSwapchain {
        //VARIABLES
    private:
        DevicePointer device;
        VkSwapchainKHR swapchain{ VK_NULL_HANDLE };

        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

        std::vector<VkImage> images;
        std::vector<std::shared_ptr<VKImageView>> imageViews;

        //FUNCTIONS
    public:
        VKSwapchain() = delete;
        VKSwapchain(DevicePointer device, VkSwapchainKHR swapchain, VkFormat swapChainImageFormat, VkExtent2D swapChainExtent);

        VKSwapchain(VKSwapchain const &other) = delete;
        VKSwapchain(VKSwapchain &&other) noexcept;

        VKSwapchain &operator=(VKSwapchain const &other) = delete;
        VKSwapchain &operator=(VKSwapchain &&other) noexcept;

        ~VKSwapchain();

        std::pair<uint32_t, Result> aquireNextImage(GhaSemaphore const *availableSemaphore) override;
        
        GhaImage::Format getImageFormat() const override;
        vec2ui getSize() const override;

        std::vector<std::shared_ptr<GhaImageView>> getImageViews() const override;

        VkSwapchainKHR getSwapchain() const;
    };
}