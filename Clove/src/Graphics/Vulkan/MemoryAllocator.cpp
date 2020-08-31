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

        chunks.emplace_back(std::make_unique<Chunk>(0, size, memory));
    }

    MemoryAllocator::Block::Block(Block&& other) noexcept
        : device(other.device)
        , memory(other.memory)
        , size(other.size)
        , memoryTypeIndex(other.memoryTypeIndex)
        , chunks(std::move(other.chunks)) {
        other.memory = VK_NULL_HANDLE;//Make sure the moved block no longer points to our memory
    }

    MemoryAllocator::Block& MemoryAllocator::Block::operator=(Block&& other) noexcept {
        device          = other.device;
        memory          = other.memory;
        size            = other.size;
        memoryTypeIndex = other.memoryTypeIndex;
        chunks          = std::move(other.chunks);

        other.memory    = VK_NULL_HANDLE;//Make sure the moved block no longer points to our memory

        return *this;
    }

    MemoryAllocator::Block::~Block() {
        vkFreeMemory(device, memory, nullptr);
    }

    const MemoryAllocator::Chunk* MemoryAllocator::Block::allocate(const VkDeviceSize size, const VkDeviceSize alignment) {
        for(size_t i = 0; i < std::size(chunks); ++i) {
            if(chunks[i]->free) {
                const VkDeviceSize remainingAlignment  = chunks[i]->offset % alignment;                               //How much we're off from the alignment
                const VkDeviceSize alignmentCorrection = remainingAlignment != 0 ? alignment - remainingAlignment : 0;//How much we compensate for to achieve a multiple of alignment

                if(const VkDeviceSize chunkSize = chunks[i]->size - alignmentCorrection; chunkSize >= size) {
                    const auto prevOffset = chunks[i]->offset;

                    chunks[i]->offset += alignmentCorrection;
                    chunks[i]->size = chunkSize;
                    chunks[i]->free = false;

                    //Add back the extra bits to the chunk to the left
                    if(const auto extraBits = chunks[i]->offset - prevOffset; extraBits > 0) {
                        chunks[i - 1]->size += extraBits;
                    }

                    //If we have room left in the chunk, split it and put the excess back in the list.
                    if(const VkDeviceSize remainingSize = chunks[i]->size - size; remainingSize > 0) {
                        auto newChunk = std::make_unique<Chunk>(chunks[i]->offset + size, remainingSize, memory);
                        chunks.insert(std::begin(chunks) + i + 1, std::move(newChunk));

                        chunks[i]->size = size;
                    }

                    return chunks[i].get();
                }
            }
        }

        return nullptr;
    }

    void MemoryAllocator::Block::free(const Chunk* chunk) {
        //chunk->free = true;
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

    const MemoryAllocator::Chunk* MemoryAllocator::allocate(const VkMemoryRequirements& memoryRequirements, VkMemoryPropertyFlags properties) {
        const uint32_t memoryTypeIndex = getMemoryTypeIndex(memoryRequirements.memoryTypeBits, properties, device.getPhysical());

        const Chunk* freeChunk{ nullptr };
        for(auto& block : memoryBlocks) {
            if(block.getMemoryTypeIndex() == memoryTypeIndex) {
                freeChunk = block.allocate(memoryRequirements.size, memoryRequirements.alignment);

                if(freeChunk != nullptr) {
                    break;
                }
            }
        }

        if(freeChunk == nullptr) {
            //Make sure if allocate a new block that's big enough
            const VkDeviceSize size = std::max(memoryRequirements.size, blockSize);
            memoryBlocks.emplace_back(device.get(), size, memoryTypeIndex);
            freeChunk = memoryBlocks.back().allocate(memoryRequirements.size, memoryRequirements.alignment);
            GARLIC_ASSERT(freeChunk != nullptr, "{0}: Newly allocated Block does not have enough room", GARLIC_FUNCTION_NAME_PRETTY);
        }

        return freeChunk;
    }

    void MemoryAllocator::free(const Chunk* chunk) {
        //TODO
    }
}