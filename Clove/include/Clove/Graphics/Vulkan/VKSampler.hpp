#pragma once

//TODO: move
#include <vulkan/vulkan.h>

namespace clv::gfx {
    enum class SamplerFilter{
        Nearest,
        Linear
    };
    enum class SamplerAddressMode{
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder
    };

    struct SamplerDescriptor {
        SamplerFilter minFilter;
        SamplerFilter magFilter;
        SamplerAddressMode addressModeU;
        SamplerAddressMode addressModeV;
        SamplerAddressMode addressModeW;
        bool enableAnisotropy = false;
        float maxAnisotropy   = 1.0f;
    };
}

namespace clv::gfx::vk {
    class VKSampler {
        //VARIABLES
    private:
        VkDevice device;
        VkSampler sampler;

        //FUNCTIONS
    public:
        //TODO: Ctors
        VKSampler(VkDevice device, SamplerDescriptor descriptor);
        ~VKSampler();

        VkSampler getSampler() const;
    };
}