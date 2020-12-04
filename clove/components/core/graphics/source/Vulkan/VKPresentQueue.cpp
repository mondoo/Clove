#include "Clove/Graphics/Vulkan/VKPresentQueue.hpp"

#include "Clove/Graphics/Vulkan/VKFence.hpp"
#include "Clove/Graphics/Vulkan/VKResult.hpp"
#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"
#include "Clove/Graphics/Vulkan/VKSwapchain.hpp"

#include <Clove/Cast.hpp>

namespace garlic::clove {
    VKPresentQueue::VKPresentQueue(DevicePointer device, VkQueue queue)
        : device{ std::move(device) }
        , queue{ queue } {
    }

    VKPresentQueue::VKPresentQueue(VKPresentQueue &&other) noexcept = default;

    VKPresentQueue &VKPresentQueue::operator=(VKPresentQueue &&other) noexcept = default;

    VKPresentQueue::~VKPresentQueue() = default;

    Result VKPresentQueue::present(PresentInfo const &presentInfo) {
        //Wait semaphores
        const size_t waitSemaphoreCount = std::size(presentInfo.waitSemaphores);
        std::vector<VkSemaphore> waitSemaphores(waitSemaphoreCount);
        for(size_t i = 0; i < waitSemaphoreCount; ++i) {
            waitSemaphores[i] = polyCast<VKSemaphore>(presentInfo.waitSemaphores[i].get())->getSemaphore();
        }

        VkSwapchainKHR swapchains[] = { polyCast<VKSwapchain>(presentInfo.swapChain.get())->getSwapchain() };
        uint32_t imageIndices[]     = { presentInfo.imageIndex };

        VkPresentInfoKHR vkpresentInfo{
            .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .waitSemaphoreCount = static_cast<uint32_t>(waitSemaphoreCount),
            .pWaitSemaphores    = std::data(waitSemaphores),
            .swapchainCount     = 1,
            .pSwapchains        = swapchains,
            .pImageIndices      = imageIndices,
        };

        VkResult const result{ vkQueuePresentKHR(queue, &vkpresentInfo) };

        return convertResult(result);
    }
}