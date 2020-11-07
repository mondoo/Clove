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
        MemoryAllocator::Chunk const *allocatedBlock{ nullptr };

        //FUNCTIONS
    public:
        VKBuffer() = delete;
        VKBuffer(DevicePointer device, Descriptor descriptor, QueueFamilyIndices const &familyIndices, std::shared_ptr<MemoryAllocator> memoryAllocator);

        VKBuffer(VKBuffer const &other) = delete;
        VKBuffer(VKBuffer &&other) noexcept;

        VKBuffer &operator=(const VKBuffer &other) = delete;
        VKBuffer &operator=(VKBuffer &&other) noexcept;

        ~VKBuffer();

        void write(void const *data, size_t const offset, size_t const size) override;

        void read(void *data, size_t const offset, size_t const size) override;

        VkBuffer getBuffer() const;
    };
}