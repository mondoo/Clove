#include "Clove/Graphics/Vulkan/MemoryAllocator.hpp"

namespace clv::gfx::vk {
    static uint32_t getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice) {
        VkPhysicalDeviceMemoryProperties memoryType{};
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryType);

        for(uint32_t i = 0; i < memoryType.memoryTypeCount; ++i) {
            if((typeBits & (1 << i)) != 0 && (memoryType.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        GARLIC_ASSERT(false, "{0}: Failed to find the specified index", GARLIC_FUNCTION_NAME);
        return -1;
    }

    MemoryAllocator::Block::Block(VkDevice device, VkDeviceSize size, uint32_t memoryTypeIndex)
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

    void MemoryAllocator::allocate(VkBuffer buffer, VkDeviceSize allocationSize, VkMemoryPropertyFlags properties) {
        VkMemoryRequirements memoryRequirements{};
        vkGetBufferMemoryRequirements(device.get(), buffer, &memoryRequirements);

        const uint32_t memoryTypeIndex = getMemoryTypeIndex(memoryRequirements.memoryTypeBits, properties, device.getPhysical());
        
        if(allocationSize > blockSize) {
            //TODO: Allocate a block specifically for this buffer
        }else if(std::find(std::begin(memoryBlocks), std::end(memoryBlocks), memoryTypeIndex) == std::end(memoryBlocks)) {
            memoryBlocks[memoryTypeIndex].emplace_back(device.get(), blockSize, memoryTypeIndex);
        }
        
        //vkBindBufferMemory(device.get(), buffer, bufferMemory, 0);

       
    }

    void MemoryAllocator::allocate(VkImage image, VkDeviceSize allocationSize, VkMemoryPropertyFlags properties) {
        //TODO
    }
}