#pragma once

#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class MemoryAllocator {
        //TYPES
    public:
        /**
         * @brief A portion of allocated memory inside a VkDeviceMemory.
         */
        struct Chunk {
            VkDeviceSize offset{ 0 }; /**< Offset in to the memory this chunk occupies. */
            VkDeviceSize size{ 0 };   /**< Size of this memory chunk. */

            VkDeviceMemory memory{ VK_NULL_HANDLE };

            bool free{ true };

            Chunk(VkDeviceSize offset, VkDeviceSize size, VkDeviceMemory memory)
                : offset(offset)
                , size(size)
                , memory(memory) {
            }

            friend bool operator==(const Chunk& lhs, const Chunk& rhs) {
                return lhs.offset == rhs.offset &&
                    lhs.size == rhs.size &&
                    lhs.memory == rhs.memory &&
                    lhs.free == rhs.free;
            }
        };

    private:
        /**
         * @brief Represents a single VkDeviceMemory allocation.
         */
        class Block {
            //VARIABLES
        private:
            VkDevice device{ VK_NULL_HANDLE };

            VkDeviceMemory memory{ VK_NULL_HANDLE };
            VkDeviceSize size{ 0 };
            uint32_t memoryTypeIndex{ 0 };

            std::list<Chunk> chunks;

            //FUNCTIONS
        public:
            Block() = delete;
            Block(VkDevice device, VkDeviceSize size, uint32_t memoryTypeIndex);

            Block(const Block& other) = delete;
            Block(Block&& other) noexcept;

            Block& operator=(const Block& other) = delete;
            Block& operator=(Block&& other) noexcept;

            ~Block();

            const Chunk* allocate(const VkDeviceSize size, const VkDeviceSize alignment);
            bool free(const Chunk*& chunkPtr);

            //TODO: Move to .inl
            inline VkDeviceSize getSize() const {
                return size;
            }

            inline uint32_t getMemoryTypeIndex() const {
                return memoryTypeIndex;
            }
        };

        //VARIABLES
    private:
        static constexpr VkDeviceSize blockSize = 256 * 1024 * 1024;//256MB

        DevicePointer device;

        std::vector<Block> memoryBlocks;

        //FUNCTIONS
    public:
        MemoryAllocator() = delete;
        MemoryAllocator(DevicePointer device);

        MemoryAllocator(const MemoryAllocator& other) = delete;
        MemoryAllocator(MemoryAllocator&& other) noexcept;

        MemoryAllocator& operator=(const MemoryAllocator& other) = delete;
        MemoryAllocator& operator=(MemoryAllocator&& other);

        ~MemoryAllocator();

        const Chunk* allocate(const VkMemoryRequirements& memoryRequirements, VkMemoryPropertyFlags properties);
        void free(const Chunk*& chunk);
    };
}