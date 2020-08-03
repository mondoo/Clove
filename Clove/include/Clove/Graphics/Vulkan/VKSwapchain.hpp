#pragma once

#include "Clove/Graphics/Swapchain.hpp"
#include "Clove/Graphics/Vulkan/VulkanHelpers.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx {
    class VKImageView;
}

namespace clv::gfx::vk {
    class VKSwapchain : public Swapchain {
        //VARIABLES
    private:
        VkDevice device          = VK_NULL_HANDLE;
        VkSwapchainKHR swapChain = VK_NULL_HANDLE;

        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

        std::vector<VkImage> images;
        std::vector<std::shared_ptr<VKImageView>> imageViews;

        //FUNCTIONS
    public:
        VKSwapchain() = delete;
        VKSwapchain(VkDevice device, SwapchainSupportDetails supportDetails, VkSurfaceKHR surface, const QueueFamilyIndices& familyIndices, Descriptor descriptor);

        VKSwapchain(const VKSwapchain& other) = delete;
        VKSwapchain(VKSwapchain&& other) noexcept;

        VKSwapchain& operator=(const VKSwapchain& other) = delete;
        VKSwapchain& operator=(VKSwapchain&& other) noexcept;

        ~VKSwapchain();

        ImageFormat getImageFormat() const override;
        clv::mth::vec2ui getExtent() const override;

        Result aquireNextImage(const Semaphore* semaphore, uint32_t& outImageIndex) override;

        std::vector<std::shared_ptr<GraphicsImageView>> getImageViews() const override;

        VkSwapchainKHR getSwapchain() const;
    };
}