#pragma once

#include "Clove/Graphics/GraphicsCommandBuffer.hpp"
#include "Clove/Graphics/GraphicsTypes.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKGraphicsCommandBuffer : public GraphicsCommandBuffer{
        //VARIABLES
    private:
        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;

        //FUNCTIONS
    public:
        VKGraphicsCommandBuffer() = delete;
        VKGraphicsCommandBuffer(VkCommandBuffer commandBuffer);

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
        void bindDescriptorSet(DescriptorSet& descriptorSet, const PipelineObject& pipeline) override;

        void drawIndexed(const size_t indexCount) override;

        void transitionImageLayout(GraphicsImage& image, ImageLayout previousLayout, ImageLayout newLayout) override;

        VkCommandBuffer getCommandBuffer() const;
    };
}