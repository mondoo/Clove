#include "Clove/Graphics/Vulkan/VKGraphicsCommandBuffer.hpp"

#include "Clove/Graphics/Vulkan/VKBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKCommandBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKDescriptorSet.hpp"
#include "Clove/Graphics/Vulkan/VKFramebuffer.hpp"
#include "Clove/Graphics/Vulkan/VKGraphicsResource.hpp"
#include "Clove/Graphics/Vulkan/VKImage.hpp"
#include "Clove/Graphics/Vulkan/VKMemoryBarrier.hpp"
#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"
#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"
#include "Clove/Graphics/Vulkan/VKShader.hpp"
#include "Clove/Utils/Cast.hpp"

namespace clv::gfx::vk {
    static VkIndexType getIndexType(IndexType garlicType) {
        switch(garlicType) {
            case IndexType::Uint16:
                return VK_INDEX_TYPE_UINT16;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown index type", GARLIC_FUNCTION_NAME);
                return VK_INDEX_TYPE_UINT16;
        }
    }

    VKGraphicsCommandBuffer::VKGraphicsCommandBuffer(VkCommandBuffer commandBuffer, QueueFamilyIndices queueFamilyIndices)
        : commandBuffer(commandBuffer)
        , queueFamilyIndices(std::move(queueFamilyIndices)) {
    }

    VKGraphicsCommandBuffer::VKGraphicsCommandBuffer(VKGraphicsCommandBuffer&& other) noexcept = default;

    VKGraphicsCommandBuffer& VKGraphicsCommandBuffer::operator=(VKGraphicsCommandBuffer&& other) noexcept = default;

    VKGraphicsCommandBuffer::~VKGraphicsCommandBuffer() = default;

    void VKGraphicsCommandBuffer::beginRecording(CommandBufferUsage usageFlag) {
        VkCommandBufferBeginInfo beginInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags            = getCommandBufferUsageFlags(usageFlag),
            .pInheritanceInfo = nullptr,
        };

        if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to begin recording command buffer");
        }
    }

    void VKGraphicsCommandBuffer::endRecording() {
        if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to end recording command buffer");
        }
    }

    void VKGraphicsCommandBuffer::beginRenderPass(RenderPass& renderPass, Framebuffer& frameBuffer, const RenderArea& renderArea, std::span<ClearValue> clearValues) {
        std::vector<VkClearValue> vkClearValues(std::size(clearValues));
        for(uint32_t index = 0; auto& clearValue : clearValues) {
            const auto& colour       = clearValue.colour;
            const auto& depthStencil = clearValue.depthStencil;

            vkClearValues[index].color        = { colour.r, colour.g, colour.b, colour.a };
            vkClearValues[index].depthStencil = { depthStencil.depth, depthStencil.stencil };

            ++index;
        }

        VkRenderPassBeginInfo renderPassInfo{
            .sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass  = polyCast<VKRenderPass>(&renderPass)->getRenderPass(),
            .framebuffer = polyCast<VKFramebuffer>(&frameBuffer)->getFrameBuffer(),
            .renderArea  = {
                .offset = { renderArea.origin.x, renderArea.origin.y },
                .extent = { renderArea.size.x, renderArea.size.y },
            },
            .clearValueCount = static_cast<uint32_t>(std::size(vkClearValues)),
            .pClearValues    = std::data(vkClearValues),
        };

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void VKGraphicsCommandBuffer::endRenderPass() {
        vkCmdEndRenderPass(commandBuffer);
    }

    void VKGraphicsCommandBuffer::bindVertexBuffer(GraphicsBuffer& vertexBuffer, const uint32_t binding) {
        VkBuffer buffers[]     = { polyCast<VKBuffer>(&vertexBuffer)->getBuffer() };
        VkDeviceSize offsets[] = { 0 };

        vkCmdBindVertexBuffers(commandBuffer, binding, 1, buffers, offsets);
    }

    void VKGraphicsCommandBuffer::bindIndexBuffer(GraphicsBuffer& indexBuffer, IndexType indexType) {
        vkCmdBindIndexBuffer(commandBuffer, polyCast<VKBuffer>(&indexBuffer)->getBuffer(), 0, getIndexType(indexType));
    }

    void VKGraphicsCommandBuffer::bindPipelineObject(PipelineObject& pipelineObject) {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, polyCast<VKPipelineObject>(&pipelineObject)->getPipeline());
    }

    void VKGraphicsCommandBuffer::bindDescriptorSet(DescriptorSet& descriptorSet, const PipelineObject& pipeline, const uint32_t setNum) {
        VkDescriptorSet sets[] = { polyCast<VKDescriptorSet>(&descriptorSet)->getDescriptorSet() };

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, polyCast<const VKPipelineObject>(&pipeline)->getLayout(), setNum, 1, sets, 0, nullptr);
    }

    void VKGraphicsCommandBuffer::pushConstant(PipelineObject& pipelineObject, const Shader::Stage stage, const uint32_t size, const void* data) {
        vkCmdPushConstants(commandBuffer, polyCast<VKPipelineObject>(&pipelineObject)->getLayout(), VKShader::convertStage(stage), 0, size, data);
    }

    void VKGraphicsCommandBuffer::drawIndexed(const size_t indexCount) {
        vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
    }

    void VKGraphicsCommandBuffer::bufferMemoryBarrier(GraphicsBuffer& buffer, const BufferMemoryBarrierInfo& barrierInfo, PipelineObject::Stage sourceStage, PipelineObject::Stage destinationStage) {
        const uint32_t sourceFamilyIndex      = getQueueFamilyIndex(barrierInfo.sourceQueue, queueFamilyIndices);
        const uint32_t destinationFamilyIndex = getQueueFamilyIndex(barrierInfo.destinationQueue, queueFamilyIndices);

        VkBufferMemoryBarrier barrier{
            .sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
            .pNext               = nullptr,
            .srcAccessMask       = convertAccessFlags(barrierInfo.sourceAccess),
            .dstAccessMask       = convertAccessFlags(barrierInfo.destinationAccess),
            .srcQueueFamilyIndex = sourceFamilyIndex,
            .dstQueueFamilyIndex = destinationFamilyIndex,
            .buffer              = polyCast<VKBuffer>(&buffer)->getBuffer(),
            .offset              = 0,
            .size                = VK_WHOLE_SIZE,
        };

        const VkPipelineStageFlags vkSourceStage      = VKPipelineObject::convertStage(sourceStage);
        const VkPipelineStageFlags vkDestinationStage = VKPipelineObject::convertStage(destinationStage);

        vkCmdPipelineBarrier(commandBuffer, vkSourceStage, vkDestinationStage, 0, 0, nullptr, 1, &barrier, 0, nullptr);
    }

    void VKGraphicsCommandBuffer::imageMemoryBarrier(GraphicsImage& image, const ImageMemoryBarrierInfo& barrierInfo, PipelineObject::Stage sourceStage, PipelineObject::Stage destinationStage) {
        const uint32_t sourceFamilyIndex      = getQueueFamilyIndex(barrierInfo.sourceQueue, queueFamilyIndices);
        const uint32_t destinationFamilyIndex = getQueueFamilyIndex(barrierInfo.destinationQueue, queueFamilyIndices);

        VkImageMemoryBarrier barrier{
            .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext               = nullptr,
            .srcAccessMask       = convertAccessFlags(barrierInfo.sourceAccess),
            .dstAccessMask       = convertAccessFlags(barrierInfo.destinationAccess),
            .oldLayout           = VKImage::convertLayout(barrierInfo.oldImageLayout),
            .newLayout           = VKImage::convertLayout(barrierInfo.newImageLayout),
            .srcQueueFamilyIndex = sourceFamilyIndex,
            .dstQueueFamilyIndex = destinationFamilyIndex,
            .image               = polyCast<VKImage>(&image)->getImage(),
            .subresourceRange    = {
                .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,//TODO: Handle other aspect mask,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = 1,
            },
        };

        const VkPipelineStageFlags vkSourceStage      = VKPipelineObject::convertStage(sourceStage);
        const VkPipelineStageFlags vkDestinationStage = VKPipelineObject::convertStage(destinationStage);

        vkCmdPipelineBarrier(commandBuffer, vkSourceStage, vkDestinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    }

    VkCommandBuffer VKGraphicsCommandBuffer::getCommandBuffer() const {
        return commandBuffer;
    }
}