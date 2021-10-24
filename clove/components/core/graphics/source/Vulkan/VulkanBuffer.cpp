#include "Clove/Graphics/Vulkan/VulkanBuffer.hpp"

#include "Clove/Graphics/Vulkan/VulkanResource.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include <Clove/Log/Log.hpp>

namespace clove {
    VulkanBuffer::VulkanBuffer(DevicePointer device, VkBuffer buffer, Descriptor descriptor, MemoryAllocator::Chunk const *allocatedBlock, std::shared_ptr<MemoryAllocator> memoryAllocator)
        : device{ std::move(device) }
        , buffer{ buffer }
        , descriptor{ descriptor }
        , allocatedBlock{ allocatedBlock }
        , memoryAllocator{ std::move(memoryAllocator) } {
    }

    VulkanBuffer::VulkanBuffer(VulkanBuffer &&other) noexcept = default;

    VulkanBuffer &VulkanBuffer::operator=(VulkanBuffer &&other) noexcept = default;

    VulkanBuffer::~VulkanBuffer() {
        vkDestroyBuffer(device.get(), buffer, nullptr);
        memoryAllocator->free(allocatedBlock);
    }

    GhaBuffer::Descriptor const &VulkanBuffer::getDescriptor() const {
        return descriptor;
    }

    void VulkanBuffer::write(void const *data, size_t const offset, size_t const size) {
        void *cpuAccessibleMemory{ nullptr };

        vkMapMemory(device.get(), allocatedBlock->memory, allocatedBlock->offset + offset, size, 0, &cpuAccessibleMemory);
        memcpy(cpuAccessibleMemory, data, size);
        vkUnmapMemory(device.get(), allocatedBlock->memory);
    }

    void VulkanBuffer::read(void *data, size_t const offset, size_t const size) {
        void *cpuAccessibleMemory{ nullptr };

        vkMapMemory(device.get(), allocatedBlock->memory, allocatedBlock->offset + offset, size, 0, &cpuAccessibleMemory);
        memcpy(data, cpuAccessibleMemory, size);
        vkUnmapMemory(device.get(), allocatedBlock->memory);
    }

    VkBuffer VulkanBuffer::getBuffer() const {
        return buffer;
    }
}