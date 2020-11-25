#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"

#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    VKSemaphore::VKSemaphore(DevicePointer device)
        : device(std::move(device)) {
        VkSemaphoreCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        if(vkCreateSemaphore(this->device.get(), &createInfo, nullptr, &semaphore) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, LogLevel::Error, "Failed to create semaphore");
        }
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