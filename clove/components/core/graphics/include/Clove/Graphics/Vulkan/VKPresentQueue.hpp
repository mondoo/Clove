#pragma once

#include "Clove/Graphics/PresentQueue.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    class VKPresentQueue : public PresentQueue {
        //VARIABLES
    private:
        DevicePointer device;
        VkQueue queue{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        VKPresentQueue() = delete;
        VKPresentQueue(DevicePointer device, uint32_t queueFamilyIndex);

        VKPresentQueue(const VKPresentQueue& other) = delete;
        VKPresentQueue(VKPresentQueue&& other) noexcept;

        VKPresentQueue& operator=(const VKPresentQueue& other) = delete;
        VKPresentQueue& operator=(VKPresentQueue&& other) noexcept;

        ~VKPresentQueue();

        Result present(const PresentInfo& presentInfo);
    };
}