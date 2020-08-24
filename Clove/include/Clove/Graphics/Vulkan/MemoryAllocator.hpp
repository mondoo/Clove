#pragma once

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class MemoryAllocator {
        //VARIABLES
    private:
        /*
        - Would probably allocate 256mb blocks at a time.
        - Might need separate device memorys for video / system.
        - Type of memory is chosen by buffer / image requirements so might need several of these
        */

        VkDeviceMemory bufferMemory{ VK_NULL_HANDLE };
        VkDeviceMemory imageMemory{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
    };
}