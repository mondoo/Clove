#include "Clove/Graphics/Vulkan/VKFence.hpp"

namespace clv::gfx::vk {
    VKFence::VKFence(DevicePointer device, Descriptor descriptor)
        : device(std::move(device)) {
        VkFenceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        createInfo.flags = descriptor.signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

        vkCreateFence(this->device.get(), &createInfo, nullptr, &fence);
    }

    VKFence::VKFence(VKFence&& other) noexcept = default;

    VKFence& VKFence::operator=(VKFence&& other) noexcept = default;

    VKFence::~VKFence() {
        vkDestroyFence(device.get(), fence, nullptr);
    }

    void VKFence::wait() {
        vkWaitForFences(device.get(), 1, &fence, VK_TRUE, UINT64_MAX);
    }

    void VKFence::reset() {
        vkResetFences(device.get(), 1, &fence);
    }

    VkFence VKFence::getFence() const {
        return fence;
    }
}