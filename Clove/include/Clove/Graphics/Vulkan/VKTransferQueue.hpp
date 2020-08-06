#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/TransferQueue.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKTransferQueue : public TransferQueue {
        //VARIABLES
    private:
        VkDevice device           = VK_NULL_HANDLE;
        VkQueue queue             = VK_NULL_HANDLE;
        VkCommandPool commandPool = VK_NULL_HANDLE;

        //FUNCTIONS
    public:
        //TODO: Ctors
        VKTransferQueue() = delete;
        VKTransferQueue(VkDevice device, uint32_t queueFamilyIndex, CommandQueueDescriptor descriptor);

        ~VKTransferQueue();

        std::unique_ptr<TransferCommandBuffer> allocateCommandBuffer() override;
        void freeCommandBuffer(TransferCommandBuffer& buffer) override;

        void submit(const TransferSubmitInfo& submitInfo) override;
    };
}