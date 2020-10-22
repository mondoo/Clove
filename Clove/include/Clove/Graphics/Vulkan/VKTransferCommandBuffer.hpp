#pragma once

#include "Clove/Graphics/TransferCommandBuffer.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKTransferCommandBuffer : public TransferCommandBuffer {
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
        VKTransferCommandBuffer &operator                                        =(VKTransferCommandBuffer &&other) noexcept;

        ~VKTransferCommandBuffer();

        void beginRecording(CommandBufferUsage usageFlag) override;
        void endRecording() override;

        void copyBufferToBuffer(GraphicsBuffer &source, size_t const sourceOffset, GraphicsBuffer &destination, size_t const destinationOffset, size_t const sizeBytes) override;
        void copyBufferToImage(GraphicsBuffer &source, size_t const sourceOffset, GraphicsImage &destination, GraphicsImage::Layout destinationLayout, mth::vec3i const &destinationOffset, mth::vec3ui const &destinationExtent) override;

        void bufferMemoryBarrier(GraphicsBuffer &buffer, BufferMemoryBarrierInfo const &barrierInfo, PipelineObject::Stage sourceStage, PipelineObject::Stage destinationStage) override;
        void imageMemoryBarrier(GraphicsImage &image, ImageMemoryBarrierInfo const &barrierInfo, PipelineObject::Stage sourceStage, PipelineObject::Stage destinationStage) override;

        VkCommandBuffer getCommandBuffer() const;
    };
}