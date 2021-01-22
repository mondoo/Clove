#pragma once

#include "Clove/Graphics/GhaSampler.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    class VKSampler : public GhaSampler {
        //VARIABLES
    private:
        DevicePointer device;

        VkSampler sampler{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        VKSampler() = delete;
        VKSampler(DevicePointer device, VkSampler sampler);

        VKSampler(VKSampler const& other) = delete;
        VKSampler(VKSampler&& other) noexcept;

        VKSampler& operator=(VKSampler const& other) = delete;
        VKSampler& operator=(VKSampler&& other) noexcept;

        ~VKSampler();

        inline VkSampler getSampler() const;
    };
}

#include "VKSampler.inl"