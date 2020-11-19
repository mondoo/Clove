#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"

#include <Root/Log/Log.hpp>

namespace clv::gfx::vk {
    VKSemaphore::VKSemaphore(DevicePointer device)
        : device(std::move(device)) {
        VkSemaphoreCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        if(vkCreateSemaphore(this->device.get(), &createInfo, nullptr, &semaphore) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, garlic::LogLevel::Error, "Failed to create semaphore");
        }
    }

    VKSemaphore::VKSemaphore(VKSemaphore &&other) noexcept = default;

    VKSemaphore &VKSemaphore::operator=(VKSemaphore &&other) noexcept = default;

    void VKSemaphore::wait() {
        VkSemaphoreWaitInfo waitInfo{
            .sType          = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO,
            .semaphoreCount = 1,
            .pSemaphores    = &semaphore,
        };

        vkWaitSemaphores(device.get(), &waitInfo, UINT64_MAX);
    }

    void VKSemaphore::signal() {
        VkSemaphoreSignalInfo signalInfo{
            .sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO,
            .semaphore = semaphore,
            .value     = 1,
        };

        vkSignalSemaphore(device.get(), &signalInfo);
    }

    VKSemaphore::~VKSemaphore() {
        vkDestroySemaphore(device.get(), semaphore, nullptr);
    }

    VkSemaphore VKSemaphore::getSemaphore() const {
        return semaphore;
    }
}