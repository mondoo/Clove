#pragma once

#include "Clove/Graphics/Fence.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    class VKFence : public Fence {
        //VARIABLES
    private:
        DevicePointer device;

        VkFence fence{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        VKFence() = delete;
        VKFence(DevicePointer device, Descriptor descriptor);

        VKFence(VKFence const &other) = delete;
        VKFence(VKFence &&other) noexcept;

        VKFence &operator=(VKFence const &other) = delete;
        VKFence &operator=(VKFence &&other) noexcept;

        ~VKFence();

        void wait() override;
        void reset() override;

        VkFence getFence() const;
    };
}