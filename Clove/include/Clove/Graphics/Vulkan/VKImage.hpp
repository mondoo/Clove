#pragma once

#include "Clove/Graphics/GraphicsImage.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKImage : public GraphicsImage {
        //VARIABLES
    private:
        VkDevice device = VK_NULL_HANDLE;
        VkImage image   = VK_NULL_HANDLE;
        VkDeviceMemory imageMemory;

        Descriptor descriptor;

        //FUNCTIONS
    public:
        VKImage(VkDevice device, VkPhysicalDevice physicalDevice, Descriptor descriptor, const QueueFamilyIndices& familyIndices);
        
        VKImage(const VKImage& other) = delete;
        VKImage(VKImage&& other) noexcept;

        VKImage& operator=(const VKImage& other) = delete;
        VKImage& operator=(VKImage&& other) noexcept;
        
        ~VKImage();

        std::unique_ptr<GraphicsImageView> createView() const override;

        VkImage getImage() const;
    };
}