#pragma once

#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"
#include "Clove/Graphics/Vulkan/VKFence.hpp"
#include "Clove/Graphics/Vulkan/VKSwapchain.hpp"
#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx {
    struct PresentInfo {
        std::vector<std::shared_ptr<vk::VKSemaphore>> waitSemaphores;

        std::shared_ptr<clv::gfx::vk::VKSwapchain> swapChain;
        uint32_t imageIndex;
    };
}

namespace clv::gfx::vk{
    //Allows for presentation of swapchains
    class VKPresentQueue {
        //VARIABLES
    private:
        //uint32_t queueFamilyIndex = 0;
        VkDevice device = VK_NULL_HANDLE;
        VkQueue queue   = VK_NULL_HANDLE;

        //FUNCTIONS
    public:
        //TODO: Ctors
        VKPresentQueue() = delete;
        VKPresentQueue(VkDevice device, uint32_t queueFamilyIndex);

        ~VKPresentQueue();

        Result present(const PresentInfo& presentInfo);
    };
}