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

        VKSampler(const VKSampler& other) = delete;
        VKSampler(VKSampler&& other) noexcept;

        VKSampler& operator=(const VKSampler& other) = delete;
        VKSampler& operator=(VKSampler&& other) noexcept;

        ~VKSampler();

        VkSampler getSampler() const;
    };
}