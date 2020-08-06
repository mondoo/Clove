#pragma once

#include "Clove/Graphics/Vulkan/VKTransferCommandBuffer.hpp"

namespace clv::gfx {
    struct TransferSubmitInfo {
        std::vector<std::shared_ptr<vk::VKTransferCommandBuffer>> commandBuffers;
    };
}

namespace clv::gfx::vk {
    //Creates buffers for encoding transfer commands
    class VKTransferQueue {
        //VARIABLES
    private:
        //uint32_t queueFamilyIndex = 0;
        VkDevice device           = VK_NULL_HANDLE;
        VkQueue queue             = VK_NULL_HANDLE;
        VkCommandPool commandPool = VK_NULL_HANDLE;

        //FUNCTIONS
    public:
        //TODO: Ctors
        VKTransferQueue() = delete;
        VKTransferQueue(VkDevice device, uint32_t queueFamilyIndex, CommandQueueDescriptor descriptor);

        ~VKTransferQueue();

        std::unique_ptr<VKTransferCommandBuffer> allocateCommandBuffer();
        void freeCommandBuffer(VKTransferCommandBuffer& buffer);

        void submit(const TransferSubmitInfo& submitInfo);
    };
}