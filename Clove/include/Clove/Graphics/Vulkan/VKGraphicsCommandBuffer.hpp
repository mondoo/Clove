#pragma once

#include "Clove/Graphics/GraphicsCommandBuffer.hpp"
#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKGraphicsCommandBuffer : public GraphicsCommandBuffer {
        //VARIABLES
    private:
        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;

        QueueFamilyIndices queueFamilyIndices;

        //FUNCTIONS
    public:
        VKGraphicsCommandBuffer() = delete;
        VKGraphicsCommandBuffer(VkCommandBuffer commandBuffer, QueueFamilyIndices queueFamilyIndices);

        VKGraphicsCommandBuffer(const VKGraphicsCommandBuffer& other) = delete;
        VKGraphicsCommandBuffer(VKGraphicsCommandBuffer&& other) noexcept;

        VKGraphicsCommandBuffer& operator=(const VKGraphicsCommandBuffer& other) = delete;
        VKGraphicsCommandBuffer& operator=(VKGraphicsCommandBuffer&& other) noexcept;

        ~VKGraphicsCommandBuffer();

        void beginRecording(CommandBufferUsage usageFlag) override;
        void endRecording() override;

        void beginRenderPass(RenderPass& renderPass, Framebuffer& frameBuffer, const RenderArea& renderArea, const mth::vec4f& clearColour, const DepthStencilValue& depthStencilClearValue) override;
        void endRenderPass() override;

        void bindPipelineObject(PipelineObject& pipelineObject) override;
        void bindVertexBuffer(GraphicsBuffer& vertexBuffer, const uint32_t binding) override;
        void bindIndexBuffer(GraphicsBuffer& indexBuffer, IndexType indexType) override;
        void bindDescriptorSet(DescriptorSet& descriptorSet, const PipelineObject& pipeline, const uint32_t setNum) override;

        void pushConstant(PipelineObject& pipelineObject, const Shader::Stage stage, const uint32_t size, const void* data) override;

        void drawIndexed(const size_t indexCount) override;

        void bufferMemoryBarrier(GraphicsBuffer& buffer, const BufferMemoryBarrierInfo& barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage) override;
        void imageMemoryBarrier(GraphicsImage& image, const ImageMemoryBarrierInfo& barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage) override;

        VkCommandBuffer getCommandBuffer() const;
    };
}