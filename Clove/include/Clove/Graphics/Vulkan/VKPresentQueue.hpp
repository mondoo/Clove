#pragma once

#include "Clove/Graphics/PresentQueue.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKPresentQueue : public PresentQueue {
        //VARIABLES
    private:
        VkDevice device = VK_NULL_HANDLE;
        VkQueue queue   = VK_NULL_HANDLE;

        //FUNCTIONS
    public:
        VKPresentQueue() = delete;
        VKPresentQueue(VkDevice device, uint32_t queueFamilyIndex);

        VKPresentQueue(const VKPresentQueue& other) = delete;
        VKPresentQueue(VKPresentQueue&& other) noexcept;

        VKPresentQueue& operator=(const VKPresentQueue& other) = delete;
        VKPresentQueue& operator=(VKPresentQueue&& other) noexcept;

        ~VKPresentQueue();

        Result present(const PresentInfo& presentInfo);
    };
}