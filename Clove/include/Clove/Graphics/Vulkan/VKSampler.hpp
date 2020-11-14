#pragma once

#include "Clove/Graphics/Sampler.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKSampler : public Sampler {
        //VARIABLES
    private:
        DevicePointer device;

        VkSampler sampler{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        VKSampler() = delete;
        VKSampler(DevicePointer device, Descriptor descriptor);

        VKSampler(VKSampler const& other) = delete;
        VKSampler(VKSampler&& other) noexcept;

        VKSampler& operator=(VKSampler const& other) = delete;
        VKSampler& operator=(VKSampler&& other) noexcept;

        ~VKSampler();

        inline VkSampler getSampler() const;
    };
}

#include "VKSampler.inl"