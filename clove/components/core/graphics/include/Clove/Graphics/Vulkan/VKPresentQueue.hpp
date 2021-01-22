#pragma once

#include "Clove/Graphics/GhaPresentQueue.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    class VKPresentQueue : public GhaPresentQueue {
        //VARIABLES
    private:
        DevicePointer device;
        VkQueue queue{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        VKPresentQueue() = delete;
        VKPresentQueue(DevicePointer device, VkQueue queue);

        VKPresentQueue(const VKPresentQueue& other) = delete;
        VKPresentQueue(VKPresentQueue&& other) noexcept;

        VKPresentQueue& operator=(const VKPresentQueue& other) = delete;
        VKPresentQueue& operator=(VKPresentQueue&& other) noexcept;

        ~VKPresentQueue();

        Result present(const PresentInfo& presentInfo);
    };
}