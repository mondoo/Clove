#pragma once

#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class MemoryAllocator {
        //TYPES
    private:
        using MemoryType = uint32_t;
        class Block {
            //VARIABLES
        private:
            VkDevice device{ VK_NULL_HANDLE };

            VkDeviceMemory memory{ VK_NULL_HANDLE };
            VkDeviceSize size{ 0 };

            //FUNCTIONS
        public:
            Block() = delete;
            Block(VkDevice device, VkDeviceSize size, MemoryType memoryTypeIndex);

            //TODO: other ctors

            ~Block();

            //TODO: Move to .inl
            inline VkDeviceSize getSize() const{
                return size;
            }
        };

        //VARIABLES
    private:
        static constexpr size_t memorySize = 256 * 1024 * 1024;

        DevicePointer device;

        std::unordered_map<MemoryType, Block> memoryBlocks;

        //FUNCTIONS
    public:
        MemoryAllocator(DevicePointer device);
        //TODO: ctors
    };
}