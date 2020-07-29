#pragma once

#include "Clove/Graphics/GraphicsBuffer.hpp"

#include <vulkan/vulkan.hpp>

namespace clv::gfx::vk {
    struct QueueFamilyIndices;
}

namespace clv::gfx::vk {
    class VKBuffer : public GraphicsBuffer {
        //VARIABLES
    private:
        VkDevice device = VK_NULL_HANDLE;
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory bufferMemory;

        Descriptor descriptor;

        //FUNCTIONS
    public:
        VKBuffer() = delete;
        VKBuffer(VkDevice device, VkPhysicalDevice physicalDevice, Descriptor descriptor, const QueueFamilyIndices& familyIndices);

        VKBuffer(const VKBuffer& other) = delete;
        VKBuffer(VKBuffer&& other) noexcept;

        VKBuffer& operator=(const VKBuffer& other) = delete;
        VKBuffer& operator=(VKBuffer&& other) noexcept;

        ~VKBuffer();

        void map(const void* data, const size_t size) override;

        VkBuffer getBuffer() const;
    };
}