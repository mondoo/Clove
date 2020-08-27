#pragma once

#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class MemoryAllocator {
        //TYPES
    private:
        /**
         * @brief Represents a single VkDeviceMemory allocation.
         */
        class Block {
            //TYPES
        public:
            /**
             * @brief A portion of allocated memory inside a VkDeviceMemory.
             */
            struct Chunk {
                VkDeviceSize offset{ 0 };
                VkDeviceSize size{ 0 };
                VkDeviceMemory memory{ VK_NULL_HANDLE };

                bool free{ true };
            };

            //VARIABLES
        private:
            VkDevice device{ VK_NULL_HANDLE };

            VkDeviceMemory memory{ VK_NULL_HANDLE };
            VkDeviceSize size{ 0 };
            uint32_t memoryTypeIndex{ 0 };

            std::vector<Chunk> chunks;

            //FUNCTIONS
        public:
            Block() = delete;
            Block(VkDevice device, VkDeviceSize size, uint32_t memoryTypeIndex);

            //TODO: other ctors

            ~Block();

            std::optional<Chunk> allocate(const VkDeviceSize size);
            void free(Chunk chunk);

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
        MemoryAllocator(DevicePointer device);

        //TODO: ctors

        ~MemoryAllocator();

        void allocate(VkBuffer buffer, VkDeviceSize allocationSize, VkMemoryPropertyFlags properties);
        void allocate(VkImage image, VkDeviceSize allocationSize, VkMemoryPropertyFlags properties);

        //TODO: Free blocks
        //void free();

    private:

    };
}