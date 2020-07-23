#include "Clove/Graphics/Vulkan/VKSampler.hpp"

namespace clv::gfx::vk {
    static VkFilter getFilter(SamplerFilter garlicfilter) {
        switch(garlicfilter) {
            case SamplerFilter::Nearest:
                return VK_FILTER_NEAREST;
            case SamplerFilter::Linear:
                return VK_FILTER_LINEAR;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown type", GARLIC_FUNCTION_NAME);
                return VK_FILTER_NEAREST;
        }
    }

    static VkSamplerAddressMode getAddressMode(SamplerAddressMode garlicMode) {
        switch(garlicMode) {
            case SamplerAddressMode::Repeat:
                return VK_SAMPLER_ADDRESS_MODE_REPEAT;
            case SamplerAddressMode::MirroredRepeat:
                return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
            case SamplerAddressMode::ClampToEdge:
                return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            case SamplerAddressMode::ClampToBorder:
                return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown type", GARLIC_FUNCTION_NAME);
                return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        }
    }

    VKSampler::VKSampler(VkDevice device, SamplerDescriptor descriptor)
        : device(device) {
        VkSamplerCreateInfo createInfo{};
        createInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        createInfo.pNext                   = nullptr;
        createInfo.flags                   = 0;
        createInfo.magFilter               = getFilter(descriptor.magFilter);
        createInfo.minFilter               = getFilter(descriptor.minFilter);
        createInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        createInfo.addressModeU            = getAddressMode(descriptor.addressModeU);
        createInfo.addressModeV            = getAddressMode(descriptor.addressModeV);
        createInfo.addressModeW            = getAddressMode(descriptor.addressModeW);
        createInfo.mipLodBias              = 0.0f;
        createInfo.anisotropyEnable        = descriptor.enableAnisotropy ? VK_TRUE : VK_FALSE;
        createInfo.maxAnisotropy           = descriptor.maxAnisotropy;
        createInfo.compareEnable           = VK_FALSE;
        createInfo.compareOp               = VK_COMPARE_OP_ALWAYS;
        createInfo.minLod                  = 0.0f;
        createInfo.maxLod                  = 0.0f;
        createInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        createInfo.unnormalizedCoordinates = VK_FALSE;

        if(vkCreateSampler(device, &createInfo, nullptr, &sampler) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "{0}: Failed to create sampler", GARLIC_FUNCTION_NAME);
        }
    }

    VKSampler::~VKSampler() {
        vkDestroySampler(device, sampler, nullptr);
    }
}