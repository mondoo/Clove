#include "Clove/Graphics/Vulkan/VKSampler.hpp"

#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    VKSampler::VKSampler(DevicePointer device, VkSampler sampler)
        : device{ std::move(device) }
        , sampler{ sampler } {
    }

    VKSampler::VKSampler(VKSampler &&other) noexcept = default;

    VKSampler &VKSampler::operator=(VKSampler &&other) noexcept = default;

    VKSampler::~VKSampler() {
        vkDestroySampler(device.get(), sampler, nullptr);
    }
}