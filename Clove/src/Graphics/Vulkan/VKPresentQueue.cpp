#include "Clove/Graphics/Vulkan/VKPresentQueue.hpp"

#include "Clove/Graphics/Vulkan/VKFence.hpp"
#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"
#include "Clove/Graphics/Vulkan/VKSwapchain.hpp"
#include "Clove/Graphics/Vulkan/VulkanHelpers.hpp"
#include "Clove/Utils/Cast.hpp"

namespace clv::gfx::vk {
    VKPresentQueue::VKPresentQueue(DevicePointer device, uint32_t queueFamilyIndex)
        : device(std::move(device)) {
        vkGetDeviceQueue(this->device.get(), queueFamilyIndex, 0, &queue);
    }

    VKPresentQueue::VKPresentQueue(VKPresentQueue&& other) noexcept = default;

    VKPresentQueue& VKPresentQueue::operator=(VKPresentQueue&& other) noexcept = default;

    VKPresentQueue::~VKPresentQueue() = default;

    Result VKPresentQueue::present(const PresentInfo& presentInfo) {
        //Wait semaphores
        const size_t waitSemaphoreCount = std::size(presentInfo.waitSemaphores);
        std::vector<VkSemaphore> waitSemaphores(waitSemaphoreCount);
        for(size_t i = 0; i < waitSemaphoreCount; ++i) {
            waitSemaphores[i] = polyCast<VKSemaphore>(presentInfo.waitSemaphores[i].get())->getSemaphore();
        }

        VkSwapchainKHR swapchains[] = { polyCast<VKSwapchain>(presentInfo.swapChain.get())->getSwapchain() };
        uint32_t imageIndices[]     = { presentInfo.imageIndex };

        VkPresentInfoKHR vkpresentInfo{};
        vkpresentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        vkpresentInfo.waitSemaphoreCount = waitSemaphoreCount;
        vkpresentInfo.pWaitSemaphores    = std::data(waitSemaphores);
        vkpresentInfo.swapchainCount     = 1;
        vkpresentInfo.pSwapchains        = swapchains;
        vkpresentInfo.pImageIndices      = imageIndices;

        const VkResult result = vkQueuePresentKHR(queue, &vkpresentInfo);

        return convertResult(result);
    }
}