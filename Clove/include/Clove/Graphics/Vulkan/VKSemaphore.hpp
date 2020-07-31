#pragma once

#include "Clove/Graphics/Semaphore.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKSemaphore : public Semaphore {
        //VARIABLES
    private:
        VkDevice device;
        VkSemaphore semaphore;

        //FUNCTIONS
    public:
        VKSemaphore() = delete;
        VKSemaphore(VkDevice device);

        VKSemaphore(const VKSemaphore& other) = delete;
        VKSemaphore(VKSemaphore&& other) noexcept;

        VKSemaphore& operator=(const VKSemaphore& other) = delete;
        VKSemaphore& operator=(VKSemaphore&& other) noexcept;

        ~VKSemaphore();

        VkSemaphore getSemaphore() const;
    };
}