#pragma once

#include "Clove/Graphics/GraphicsImage.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"
#include "Clove/Graphics/Vulkan/MemoryAllocator.hpp"

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
        VKImage(DevicePointer device, Descriptor descriptor, const QueueFamilyIndices& familyIndices, std::shared_ptr<MemoryAllocator> memoryAllocator);
        
        VKImage(const VKImage& other) = delete;
        VKImage(VKImage&& other) noexcept;

        VKImage& operator=(const VKImage& other) = delete;
        VKImage& operator=(VKImage&& other) noexcept;
        
        ~VKImage();

        std::unique_ptr<GraphicsImageView> createView() const override;

        VkImage getImage() const;
    };
}