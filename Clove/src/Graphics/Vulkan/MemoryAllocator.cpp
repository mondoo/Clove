#include "Clove/Graphics/Vulkan/MemoryAllocator.hpp"

#include "Clove/Log.hpp"
#include <Root/Definitions.hpp>

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

        chunks.emplace_back(0, size, memory);
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

        other.memory = VK_NULL_HANDLE;//Make sure the moved block no longer points to our memory

        return *this;
    }

    MemoryAllocator::Block::~Block() {
        vkFreeMemory(device, memory, nullptr);
    }

    const MemoryAllocator::Chunk* MemoryAllocator::Block::allocate(const VkDeviceSize size, const VkDeviceSize alignment) {
        for(auto chunk = std::begin(chunks); chunk != std::end(chunks); ++chunk) {
            if(chunk->free) {
                const VkDeviceSize remainingAlignment  = chunk->offset % alignment;                                   //How much we're off from the alignment
                const VkDeviceSize alignmentCorrection = remainingAlignment != 0 ? alignment - remainingAlignment : 0;//How much we compensate for to achieve a multiple of alignment

                if(const VkDeviceSize chunkSize = chunk->size - alignmentCorrection; chunkSize >= size) {
                    const auto prevOffset = chunk->offset;

                    chunk->offset += alignmentCorrection;
                    chunk->size = chunkSize;
                    chunk->free = false;

                    //Add back the extra bits to the chunk to the left
                    if(const auto extraBits = chunk->offset - prevOffset; extraBits > 0) {
                        std::prev(chunk)->size += extraBits;
                    }

                    //If we have room left in the chunk, split it and put the excess back in the list.
                    if(const VkDeviceSize remainingSize = chunk->size - size; remainingSize > 0) {
                        chunks.insert(std::next(chunk), Chunk(chunk->offset + size, remainingSize, memory));
                        chunk->size = size;
                    }

                    return &*chunk;
                }
            }
        }

        return nullptr;
    }

    bool MemoryAllocator::Block::free(const Chunk*& chunkPtr) {
        for(auto chunk = std::begin(chunks); chunk != std::end(chunks); ++chunk) {
            if(*chunk == *chunkPtr) {
                chunk->free = true;

                bool removeIndex = false;
                bool removeRight = false;

                //Merge neighbouring chunks
                if(auto rightChunk = std::next(chunk); rightChunk != std::end(chunks) && rightChunk->free) {
                    //Merge right into us
                    chunk->size += rightChunk->size;

                    removeRight = true;
                }
                if(chunk != std::begin(chunks)) {
                    if(auto leftChunk = std::prev(chunk); leftChunk->free) {
                        //Merge ourselves into left
                        leftChunk->size += chunk->size;

                        removeIndex = true;
                    }
                }

                //Remove the right most first to preserve indices
                if(removeRight) {
                    chunks.erase(std::next(chunk));
                }
                if(removeIndex) {
                    chunks.erase(chunk);
                }

                chunkPtr = nullptr;
                return true;
            }
        }

        return false;
    }

    MemoryAllocator::MemoryAllocator(DevicePointer device)
        : device(std::move(device)) {
    }

    MemoryAllocator::MemoryAllocator(MemoryAllocator&& other) noexcept = default;

    MemoryAllocator& MemoryAllocator::operator=(MemoryAllocator&& other) = default;

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

    void MemoryAllocator::free(const Chunk*& chunk) {
        for(auto& block : memoryBlocks) {
            if(block.free(chunk)) {
                break;
            }
        }
    }
}