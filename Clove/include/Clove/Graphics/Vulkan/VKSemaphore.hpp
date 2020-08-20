#pragma once

#include "Clove/Graphics/Semaphore.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKSemaphore : public Semaphore {
        //VARIABLES
    private:
        DevicePointer device;

        VkSemaphore semaphore{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        VKSemaphore() = delete;
        VKSemaphore(DevicePointer device);

        VKSemaphore(const VKSemaphore& other) = delete;
        VKSemaphore(VKSemaphore&& other) noexcept;

        VKSemaphore& operator=(const VKSemaphore& other) = delete;
        VKSemaphore& operator=(VKSemaphore&& other) noexcept;

        ~VKSemaphore();

        VkSemaphore getSemaphore() const;
    };
}