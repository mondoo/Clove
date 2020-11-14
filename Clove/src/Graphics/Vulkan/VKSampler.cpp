#include "Clove/Graphics/Vulkan/VKSampler.hpp"

#include <Root/Definitions.hpp>
#include <Root/Log/Log.hpp>

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
        VkSamplerCreateInfo createInfo{
            .sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext                   = nullptr,
            .flags                   = 0,
            .magFilter               = getFilter(descriptor.magFilter),
            .minFilter               = getFilter(descriptor.minFilter),
            .mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR,
            .addressModeU            = getAddressMode(descriptor.addressModeU),
            .addressModeV            = getAddressMode(descriptor.addressModeV),
            .addressModeW            = getAddressMode(descriptor.addressModeW),
            .mipLodBias              = 0.0f,
            .anisotropyEnable        = static_cast<VkBool32>(descriptor.enableAnisotropy ? VK_TRUE : VK_FALSE),
            .maxAnisotropy           = descriptor.maxAnisotropy,
            .compareEnable           = VK_FALSE,
            .compareOp               = VK_COMPARE_OP_ALWAYS,
            .minLod                  = 0.0f,
            .maxLod                  = 0.0f,
            .borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
            .unnormalizedCoordinates = VK_FALSE,
        };

        if(vkCreateSampler(this->device.get(), &createInfo, nullptr, &sampler) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, garlic::LogLevel::Error, "{0}: Failed to create sampler", GARLIC_FUNCTION_NAME);
        }
    }

    VKSampler::VKSampler(VKSampler &&other) noexcept = default;

    VKSampler &VKSampler::operator=(VKSampler &&other) noexcept = default;

    VKSampler::~VKSampler() {
        vkDestroySampler(device.get(), sampler, nullptr);
    }
}