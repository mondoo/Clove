#include "Clove/Graphics/Vulkan/VKSampler.hpp"

namespace clv::gfx::vk {
    static VkFilter getFilter(Sampler::Filter garlicfilter) {
        switch(garlicfilter) {
            case Sampler::Filter::Nearest:
                return VK_FILTER_NEAREST;
            case Sampler::Filter::Linear:
                return VK_FILTER_LINEAR;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown type", GARLIC_FUNCTION_NAME);
                return VK_FILTER_NEAREST;
        }
    }

    static VkSamplerAddressMode getAddressMode(Sampler::AddressMode garlicMode) {
        switch(garlicMode) {
            case Sampler::AddressMode::Repeat:
                return VK_SAMPLER_ADDRESS_MODE_REPEAT;
            case Sampler::AddressMode::MirroredRepeat:
                return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
            case Sampler::AddressMode::ClampToEdge:
                return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            case Sampler::AddressMode::ClampToBorder:
                return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown type", GARLIC_FUNCTION_NAME);
                return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        }
    }

    VKSampler::VKSampler(DevicePointer device, Descriptor descriptor)
        : device(std::move(device)) {
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

        if(vkCreateSampler(this->device.get(), &createInfo, nullptr, &sampler) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "{0}: Failed to create sampler", GARLIC_FUNCTION_NAME);
        }
    }

    VKSampler::VKSampler(VKSampler&& other) noexcept = default;

    VKSampler& VKSampler::operator=(VKSampler&& other) noexcept = default;

    VKSampler::~VKSampler() {
        vkDestroySampler(device.get(), sampler, nullptr);
    }

    VkSampler VKSampler::getSampler() const {
        return sampler;
    }
}