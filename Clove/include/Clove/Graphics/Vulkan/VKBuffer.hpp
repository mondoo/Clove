#pragma once

#include "Clove/Graphics/GraphicsBuffer.hpp"

#include <vulkan/vulkan.hpp>

namespace clv::gfx::vk {
    struct QueueFamilyIndices;
}

namespace clv::gfx::vk {
    class VKBuffer : public GraphicsBuffer{
        //VARIABLES
    private:
        VkDevice device = VK_NULL_HANDLE;
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory bufferMemory;

        Descriptor descriptor;

        //FUNCTIONS
    public:
        //TODO: Ctors
        VKBuffer() = delete;
        VKBuffer(VkDevice device, VkPhysicalDevice physicalDevice, Descriptor descriptor, const QueueFamilyIndices& familyIndices);

        ~VKBuffer();

        void map(const void* data, const size_t size) override;

        VkBuffer getBuffer() const;
    };
}