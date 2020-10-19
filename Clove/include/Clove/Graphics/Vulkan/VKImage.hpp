#pragma once

#include "Clove/Graphics/GraphicsImage.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"
#include "Clove/Graphics/Vulkan/MemoryAllocator.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    struct QueueFamilyIndices;
}

namespace clv::gfx::vk {
    class VKImage : public GraphicsImage {
        //VARIABLES
    private:
        DevicePointer device;

        VkImage image{ VK_NULL_HANDLE };

        Descriptor descriptor;

        std::shared_ptr<MemoryAllocator> memoryAllocator;
        const MemoryAllocator::Chunk* allocatedBlock{ nullptr };

        //FUNCTIONS
    public:
        VKImage() = delete;
        VKImage(DevicePointer device, Descriptor descriptor, QueueFamilyIndices const& familyIndices, std::shared_ptr<MemoryAllocator> memoryAllocator);

        VKImage(VKImage const& other) = delete;
        VKImage(VKImage&& other) noexcept;

        VKImage& operator=(VKImage const& other) = delete;
        VKImage& operator=(VKImage&& other) noexcept;

        ~VKImage();

        std::unique_ptr<GraphicsImageView> createView() const override;

        inline VkImage getImage() const;

        static Format convertFormat(VkFormat vulkanFormat);
        static VkFormat convertFormat(Format garlicFormat);

        static VkImageLayout convertLayout(Layout garlicLayout);
    };
}

#include "VKImage.inl"