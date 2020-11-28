#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"

#include <utility>

namespace garlic::clove {
    VKSemaphore::VKSemaphore(DevicePointer device, VkSemaphore semaphore)
        : device{ std::move(device) }
        , semaphore{ semaphore } {
    }

    VKSemaphore::VKSemaphore(VKSemaphore &&other) noexcept = default;

    VKSemaphore &VKSemaphore::operator=(VKSemaphore &&other) noexcept = default;

    VKSemaphore::~VKSemaphore() {
        vkDestroySemaphore(device.get(), semaphore, nullptr);
    }

    VkSemaphore VKSemaphore::getSemaphore() const {
        return semaphore;
    }
}