#pragma once

#include "Clove/Graphics/GhaTransferCommandBuffer.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    class VKTransferCommandBuffer : public GhaTransferCommandBuffer {
        //VARIABLES
    private:
        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;

        QueueFamilyIndices queueFamilyIndices;

        //FUNCTIONS
    public:
        VKTransferCommandBuffer() = delete;
        VKTransferCommandBuffer(VkCommandBuffer commandBuffer, QueueFamilyIndices queueFamilyIndices);

        VKTransferCommandBuffer(VKTransferCommandBuffer const &other) = delete;
        VKTransferCommandBuffer(VKTransferCommandBuffer &&other) noexcept;

        VKTransferCommandBuffer &operator=(VKTransferCommandBuffer const &other) = delete;
        VKTransferCommandBuffer &operator=(VKTransferCommandBuffer &&other) noexcept;

        ~VKTransferCommandBuffer();

        void beginRecording(CommandBufferUsage usageFlag) override;
        void endRecording() override;

        void copyBufferToBuffer(GhaBuffer &source, size_t const sourceOffset, GhaBuffer &destination, size_t const destinationOffset, size_t const sizeBytes) override;
        void copyBufferToImage(GhaBuffer &source, size_t const sourceOffset, GhaImage &destination, vec3i const &destinationOffset, vec3ui const &destinationExtent) override;

        void copyImageToBuffer(GhaImage &source, vec3i const &sourceOffset, vec3ui const &sourceExtent, GhaBuffer &destination, size_t const destinationOffset) override;

        void bufferMemoryBarrier(GhaBuffer &buffer, BufferMemoryBarrierInfo const &barrierInfo, GhaPipelineObject::Stage sourceStage, GhaPipelineObject::Stage destinationStage) override;
        void imageMemoryBarrier(GhaImage &image, ImageMemoryBarrierInfo const &barrierInfo, GhaPipelineObject::Stage sourceStage, GhaPipelineObject::Stage destinationStage) override;

        VkCommandBuffer getCommandBuffer() const;
    };
}