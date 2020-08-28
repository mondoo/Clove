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

        chunks.emplace_back(std::make_unique<Chunk>(Chunk{ 0, size, memory, true }));
    }

    MemoryAllocator::Block::Block(Block&& other) noexcept = default;

    MemoryAllocator::Block& MemoryAllocator::Block::operator=(Block&& other) noexcept = default;

    MemoryAllocator::Block::~Block() {
        vkFreeMemory(device, memory, nullptr);
    }

    MemoryAllocator::Chunk* MemoryAllocator::Block::allocate(const VkDeviceSize size) {
        //TODO: Correct alignment

        for(auto& chunk : chunks) {
            if(chunk->free && chunk->size >= size) {
                if(const VkDeviceSize remainingSize = chunk->size - size; remainingSize > 0) {
                    //If we have room left in the chunk, split it and put the excess back in the list
                    chunk->size = size;
                    chunks.emplace_back(std::make_unique<Chunk>(Chunk{ size + 1, remainingSize, memory, true }));
                }

                chunk->free = false;

                return chunk.get();
            }
        }

        return nullptr;
    }

    void MemoryAllocator::Block::free(Chunk* chunk) {
        chunk->free = true;
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

    MemoryAllocator::Chunk* MemoryAllocator::allocate(const VkMemoryRequirements& memoryRequirements, VkDeviceSize allocationSize, VkMemoryPropertyFlags properties) {
        const uint32_t memoryTypeIndex = getMemoryTypeIndex(memoryRequirements.memoryTypeBits, properties, device.getPhysical());

        Chunk* freeChunk{ nullptr };
        for(auto& block : memoryBlocks) {
            if(block.getMemoryTypeIndex() == memoryTypeIndex) {
                freeChunk = block.allocate(allocationSize);

                if(freeChunk != nullptr) {
                    break;
                }
            }
        }

        if(freeChunk == nullptr) {
            //Make sure if allocate a new block that's big enough
            const VkDeviceSize size = std::max(allocationSize, blockSize);
            memoryBlocks.emplace_back(device.get(), size, memoryTypeIndex);
            freeChunk = memoryBlocks.back().allocate(size);
            GARLIC_ASSERT(freeChunk != nullptr, "{0}: Newly allocated Block does not have enough room", GARLIC_FUNCTION_NAME_PRETTY);
        }

        return freeChunk;
    }

    void MemoryAllocator::free(Chunk* chunk) {
        //TODO
    }
}