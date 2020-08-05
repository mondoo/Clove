#pragma once

#include "Clove/Graphics/Sampler.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKSampler : public Sampler {
        //VARIABLES
    private:
        VkDevice device;
        VkSampler sampler;

        //FUNCTIONS
    public:
        VKSampler() = delete;
        VKSampler(VkDevice device, Descriptor descriptor);

        VKSampler(const VKSampler& other) = delete;
        VKSampler(VKSampler&& other) noexcept;

        VKSampler& operator=(const VKSampler& other) = delete;
        VKSampler& operator=(VKSampler&& other) noexcept;

        ~VKSampler();

        VkSampler getSampler() const;
    };
}