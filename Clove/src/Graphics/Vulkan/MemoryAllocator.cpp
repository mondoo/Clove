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
        , size(size)
        , memoryTypeIndex(memoryTypeIndex) {
        VkMemoryAllocateInfo info{};
        info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        info.pNext           = nullptr;
        info.allocationSize  = size;
        info.memoryTypeIndex = memoryTypeIndex;

        vkAllocateMemory(device, &info, nullptr, &memory);

        chunks.emplace_back(0, size, true);
    }

    MemoryAllocator::Block::~Block() {
        vkFreeMemory(device, memory, nullptr);
    }

    //TODO: This should return a pointer to the chunk
    std::optional<MemoryAllocator::Block::Chunk> MemoryAllocator::Block::allocate(const VkDeviceSize size) {
        //TODO: Correct alignment

        for(auto& chunk : chunks) {
            if(chunk.free && chunk.size >= size) {
                if(const VkDeviceSize remainingSize = chunk.size - size; remainingSize > 0) {
                    //If we have room left in the chunk, split it and put the excess back in the list
                    chunk.size = size;
                    chunks.emplace_back( size + 1, remainingSize, true );
                }

                chunk.free = false;
                return chunk;
            }
        }

        return std::nullopt;
    }

    void MemoryAllocator::Block::free(Chunk chunk) {
        /*
        - check chunk before it
            - if free merge
        - check chunk after it
            - if free merge
        */
    }

    MemoryAllocator::MemoryAllocator(DevicePointer device)
        : device(std::move(device)) {
    }

    MemoryAllocator::~MemoryAllocator() = default;

    void MemoryAllocator::allocate(VkBuffer buffer, VkDeviceSize allocationSize, VkMemoryPropertyFlags properties) {
        VkMemoryRequirements memoryRequirements{};
        vkGetBufferMemoryRequirements(device.get(), buffer, &memoryRequirements);

        const uint32_t memoryTypeIndex = getMemoryTypeIndex(memoryRequirements.memoryTypeBits, properties, device.getPhysical());

        for(auto& block : memoryBlocks) {
            if(block.getMemoryTypeIndex() == memoryTypeIndex) {
                //Try and allocate from the block
            }
        }

        memoryBlocks.emplace_back(device.get(), blockSize, memoryTypeIndex);
        //Do the allocate
        


        //vkBindBufferMemory(device.get(), buffer, bufferMemory, 0);
    }

    void MemoryAllocator::allocate(VkImage image, VkDeviceSize allocationSize, VkMemoryPropertyFlags properties) {
        //TODO
    }
}