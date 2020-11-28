#include "Clove/Graphics/Vulkan/VKFence.hpp"

#include <utility>

namespace garlic::clove {
    VKFence::VKFence(DevicePointer device, VkFence fence)
        : device{ std::move(device) }
        , fence{ fence } {
    }

    VKFence::VKFence(VKFence &&other) noexcept = default;

    VKFence &VKFence::operator=(VKFence &&other) noexcept = default;

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