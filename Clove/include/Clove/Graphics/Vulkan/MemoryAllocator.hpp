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
            //VARIABLES
        private:
            VkDevice device{ VK_NULL_HANDLE };

            VkDeviceMemory memory{ VK_NULL_HANDLE };
            VkDeviceSize size{ 0 };

            //FUNCTIONS
        public:
            Block() = delete;
            Block(VkDevice device, VkDeviceSize size, uint32_t memoryTypeIndex);

            //TODO: other ctors

            ~Block();

            //TODO: Move to .inl
            inline VkDeviceSize getSize() const{
                return size;
            }
        };

        //VARIABLES
    private:
        static constexpr size_t blockSize = 256 * 1024 * 1024;

        DevicePointer device;

        std::unordered_map<uint32_t, std::vector<Block>> memoryBlocks;

        //FUNCTIONS
    public:
        MemoryAllocator(DevicePointer device);

        //TODO: ctors

        ~MemoryAllocator();

        void allocate(VkBuffer buffer, VkDeviceSize allocationSize, VkMemoryPropertyFlags properties);
        void allocate(VkImage image, VkDeviceSize allocationSize, VkMemoryPropertyFlags properties);

        //TODO: Free blocks
        //void free();
    };
}