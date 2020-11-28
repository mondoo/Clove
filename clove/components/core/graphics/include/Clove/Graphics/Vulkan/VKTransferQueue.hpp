#pragma once

#include "Clove/Graphics/TransferQueue.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    class VKTransferQueue : public TransferQueue {
        //VARIABLES
    private:
        DevicePointer device;
        VkQueue queue{ VK_NULL_HANDLE };
        VkCommandPool commandPool{ VK_NULL_HANDLE };

        QueueFamilyIndices queueFamilyIndices;

        //FUNCTIONS
    public:
        VKTransferQueue() = delete;
        VKTransferQueue(DevicePointer device, VkQueue queue, VkCommandPool commandPool, QueueFamilyIndices queueFamilyIndices);

        VKTransferQueue(VKTransferQueue const &other) = delete;
        VKTransferQueue(VKTransferQueue &&other) noexcept;

        VKTransferQueue &operator=(VKTransferQueue const &other) = delete;
        VKTransferQueue &operator=(VKTransferQueue &&other) noexcept;

        ~VKTransferQueue();

        std::unique_ptr<TransferCommandBuffer> allocateCommandBuffer() override;
        void freeCommandBuffer(TransferCommandBuffer &buffer) override;

        void submit(std::vector<TransferSubmitInfo> const &submissions, Fence const *signalFence) override;
    };
}