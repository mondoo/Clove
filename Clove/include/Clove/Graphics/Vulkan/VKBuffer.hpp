#pragma once

#include "Clove/Graphics/GraphicsBuffer.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"
#include "Clove/Graphics/Vulkan/MemoryAllocator.hpp"

#include <vulkan/vulkan.hpp>

namespace clv::gfx::vk {
    struct QueueFamilyIndices;
}

namespace clv::gfx::vk {
    class VKBuffer : public GraphicsBuffer {
        //VARIABLES
    private:
        DevicePointer device;

        VkBuffer buffer{ VK_NULL_HANDLE };

        Descriptor descriptor;

        std::shared_ptr<MemoryAllocator> memoryAllocator;
        const MemoryAllocator::Chunk* allocatedBlock{ nullptr };

        //FUNCTIONS
    public:
        VKBuffer() = delete;
        VKBuffer(DevicePointer device, Descriptor descriptor, const QueueFamilyIndices& familyIndices, std::shared_ptr<MemoryAllocator> memoryAllocator);

        VKBuffer(const VKBuffer& other) = delete;
        VKBuffer(VKBuffer&& other) noexcept;

        VKBuffer& operator=(const VKBuffer& other) = delete;
        VKBuffer& operator=(VKBuffer&& other) noexcept;

        ~VKBuffer();

        void map(const void* data, const size_t size) override;

        VkBuffer getBuffer() const;
    };
}