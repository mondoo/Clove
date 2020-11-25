#pragma once

#include "Clove/Graphics/Semaphore.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    class VKSemaphore : public Semaphore {
        //VARIABLES
    private:
        DevicePointer device;

        VkSemaphore semaphore{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        VKSemaphore() = delete;
        VKSemaphore(DevicePointer device);

        VKSemaphore(VKSemaphore const &other) = delete;
        VKSemaphore(VKSemaphore &&other) noexcept;

        VKSemaphore &operator=(VKSemaphore const &other) = delete;
        VKSemaphore &operator=(VKSemaphore &&other) noexcept;

        ~VKSemaphore();

        VkSemaphore getSemaphore() const;
    };
}