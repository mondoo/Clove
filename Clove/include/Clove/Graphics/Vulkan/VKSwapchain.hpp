#pragma once

#include "Clove/Graphics/Swapchain.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKImageView;
}

namespace clv::gfx::vk {
    class VKSwapchain : public Swapchain {
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
        VKSwapchain(DevicePointer device, SwapchainSupportDetails supportDetails, QueueFamilyIndices const &familyIndices, Descriptor descriptor);

        VKSwapchain(VKSwapchain const &other) = delete;
        VKSwapchain(VKSwapchain &&other) noexcept;

        VKSwapchain &operator=(VKSwapchain const &other) = delete;
        VKSwapchain &operator                            =(VKSwapchain &&other) noexcept;

        ~VKSwapchain();

        GraphicsImage::Format getImageFormat() const override;
        clv::mth::vec2ui getExtent() const override;

        Result aquireNextImage(Semaphore const *semaphore, uint32_t &outImageIndex) override;

        std::vector<std::shared_ptr<GraphicsImageView>> getImageViews() const override;

        VkSwapchainKHR getSwapchain() const;
    };
}