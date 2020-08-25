#include "Clove/Graphics/Vulkan/MemoryAllocator.hpp"

namespace clv::gfx::vk {
    MemoryAllocator::Block::Block(VkDevice device, VkDeviceSize size, MemoryType memoryTypeIndex)
        : device(device)
        , size(size) {
        VkMemoryAllocateInfo info{};
        info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        info.pNext           = nullptr;
        info.allocationSize  = size;
        info.memoryTypeIndex = memoryTypeIndex;

        vkAllocateMemory(device, &info, nullptr, &memory);
    }

    MemoryAllocator::Block::~Block() {
        vkFreeMemory(device, memory, nullptr);
    }

    MemoryAllocator::MemoryAllocator(DevicePointer device)
        : device(std::move(device)) {
    }

    MemoryAllocator::~MemoryAllocator() = default;

    void MemoryAllocator::allocate(VkBuffer buffer, VkDeviceSize allocationSize, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        /*
        - get a region of memory from a block
        - create a buffer?? (or just use one passed in?)
        */
        vkBindBufferMemory(device.get(), buffer, bufferMemory, 0);

       
    }

    void MemoryAllocator::allocate(VkImage image, VkDeviceSize allocationSize, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        //TODO
    }
}