#pragma once

#include "Clove/Graphics/Fence.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKFence : public Fence {
        //VARIABLES
    private:
        DevicePointer device;

        VkFence fence{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        VKFence() = delete;
        VKFence(DevicePointer device, Descriptor descriptor);

        VKFence(const VKFence& other) = delete;
        VKFence(VKFence&& other) noexcept;

        VKFence& operator=(const VKFence& other) = delete;
        VKFence& operator=(VKFence&& other) noexcept;

        ~VKFence();

        void wait() override;
        void reset() override;

        VkFence getFence() const;
    };
}