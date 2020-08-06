#include "Clove/Graphics/Vulkan/VKPresentQueue.hpp"

namespace clv::gfx::vk {
    VKPresentQueue::VKPresentQueue(VkDevice device, uint32_t queueFamilyIndex)
        : device(device) {
        vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);
    }

    VKPresentQueue::~VKPresentQueue() = default;

    Result VKPresentQueue::present(const PresentInfo& presentInfo) {
        //Wait semaphores
        const size_t waitSemaphoreCount = std::size(presentInfo.waitSemaphores);
        std::vector<VkSemaphore> waitSemaphores(waitSemaphoreCount);
        for(size_t i = 0; i < waitSemaphoreCount; ++i) {
            waitSemaphores[i] = presentInfo.waitSemaphores[i]->getSemaphore();
        }

        VkPresentInfoKHR vkpresentInfo{};
        vkpresentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        vkpresentInfo.waitSemaphoreCount = waitSemaphoreCount;
        vkpresentInfo.pWaitSemaphores    = std::data(waitSemaphores);
        vkpresentInfo.swapchainCount     = 1;
        vkpresentInfo.pSwapchains        = &presentInfo.swapChain->getSwapchain();
        vkpresentInfo.pImageIndices      = &presentInfo.imageIndex;

        const VkResult result = vkQueuePresentKHR(queue, &vkpresentInfo);

        return convertResult(result);
    }

    VKTransferQueue::VKTransferQueue(VkDevice device, uint32_t queueFamilyIndex, CommandQueueDescriptor descriptor)
        : device(device) {
        vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndex;
        poolInfo.flags            = descriptor.flags == QueueFlags::Transient ? VK_COMMAND_POOL_CREATE_TRANSIENT_BIT : 0;

        if(vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create graphics command pool");
        }
    }

    VKTransferQueue::~VKTransferQueue() {
        vkDestroyCommandPool(device, commandPool, nullptr);
    }
}