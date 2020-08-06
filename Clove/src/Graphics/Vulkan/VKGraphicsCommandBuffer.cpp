#include "Clove/Graphics/Vulkan/VKGraphicsCommandBuffer.hpp"

#include "Clove/Graphics/Vulkan/VKBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKDescriptorSet.hpp"
#include "Clove/Graphics/Vulkan/VKFramebuffer.hpp"
#include "Clove/Graphics/Vulkan/VKImage.hpp"
#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"
#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"
#include "Clove/Graphics/Vulkan/VulkanHelpers.hpp"
#include "Clove/Utils/Cast.hpp"

namespace clv::gfx::vk {
    VKGraphicsCommandBuffer::VKGraphicsCommandBuffer(VkCommandBuffer commandBuffer)
        : commandBuffer(commandBuffer) {
    }

    VKGraphicsCommandBuffer::VKGraphicsCommandBuffer(VKGraphicsCommandBuffer&& other) noexcept = default;

    VKGraphicsCommandBuffer& VKGraphicsCommandBuffer::operator=(VKGraphicsCommandBuffer&& other) noexcept = default;

    VKGraphicsCommandBuffer::~VKGraphicsCommandBuffer() = default;

    void VKGraphicsCommandBuffer::beginRecording(CommandBufferUsage usageFlag) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags            = getCommandBufferUsageFlags(usageFlag);
        beginInfo.pInheritanceInfo = nullptr;

        if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to begin recording command buffer");
        }
    }

    void VKGraphicsCommandBuffer::endRecording() {
        if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to end recording command buffer");
        }
    }

    void VKGraphicsCommandBuffer::beginRenderPass(RenderPass& renderPass, Framebuffer& frameBuffer, const RenderArea& renderArea, const mth::vec4f& clearColour, const DepthStencilValue& depthStencilClearValue) {
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color        = { clearColour.r, clearColour.g, clearColour.b, clearColour.a };
        clearValues[1].depthStencil = { depthStencilClearValue.depth, depthStencilClearValue.stencil };

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass        = polyCast<VKRenderPass>(&renderPass)->getRenderPass();
        renderPassInfo.framebuffer       = polyCast<VKFramebuffer>(&frameBuffer)->getFrameBuffer();
        renderPassInfo.renderArea.offset = { renderArea.origin.x, renderArea.origin.y };
        renderPassInfo.renderArea.extent = { renderArea.size.x, renderArea.size.y };
        renderPassInfo.clearValueCount   = std::size(clearValues);
        renderPassInfo.pClearValues      = std::data(clearValues);

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void VKGraphicsCommandBuffer::endRenderPass() {
        vkCmdEndRenderPass(commandBuffer);
    }

    void VKGraphicsCommandBuffer::bindVertexBuffer(GraphicsBuffer& vertexBuffer) {
        VkBuffer buffers[]     = { polyCast<VKBuffer>(&vertexBuffer)->getBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

    void VKGraphicsCommandBuffer::bindIndexBuffer(GraphicsBuffer& indexBuffer, IndexType indexType) {
        vkCmdBindIndexBuffer(commandBuffer, polyCast<VKBuffer>(&indexBuffer)->getBuffer(), 0, getIndexType(indexType));
    }

    void VKGraphicsCommandBuffer::bindPipelineObject(PipelineObject& pipelineObject) {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, polyCast<VKPipelineObject>(&pipelineObject)->getPipeline());
    }

    void VKGraphicsCommandBuffer::bindDescriptorSet(DescriptorSet& descriptorSet, const PipelineObject& pipeline) {
        VkDescriptorSet sets[] = { polyCast<VKDescriptorSet>(&descriptorSet)->getDescriptorSet() };
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, polyCast<VKPipelineObject>(&pipeline)->getLayout(), 0, 1, sets, 0, nullptr);
    }

    void VKGraphicsCommandBuffer::drawIndexed(const size_t indexCount) {
        vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
    }

    void VKGraphicsCommandBuffer::transitionImageLayout(GraphicsImage& image, ImageLayout previousLayout, ImageLayout newLayout) {
        const VkImageLayout vkPrevLayout = convertImageLayout(previousLayout);
        const VkImageLayout vkNextLayout = convertImageLayout(newLayout);

        //src = what happens before the barrier, dst = what needs to wait on the barrier
        //TODO: Take src and dst as params
        const auto [srcAccess, dstAccess] = getAccessFlags(vkPrevLayout, vkNextLayout);
        const auto [srcStage, dstStage]   = getStageFlags(vkPrevLayout, vkNextLayout);

        VkImageMemoryBarrier barrier{};
        barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.pNext                           = nullptr;
        barrier.srcAccessMask                   = srcAccess;
        barrier.dstAccessMask                   = dstAccess;
        barrier.oldLayout                       = vkPrevLayout;
        barrier.newLayout                       = vkNextLayout;
        barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
        barrier.image                           = polyCast<VKImage>(&image)->getImage();
        barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;//TODO: Handle other aspect masks
        barrier.subresourceRange.baseMipLevel   = 0;
        barrier.subresourceRange.levelCount     = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount     = 1;
        vkCmdPipelineBarrier(commandBuffer, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    }

    VkCommandBuffer VKGraphicsCommandBuffer::getCommandBuffer() const {
        return commandBuffer;
    }
}