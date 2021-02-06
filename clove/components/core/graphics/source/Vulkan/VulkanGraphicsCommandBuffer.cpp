#include "Clove/Graphics/Vulkan/VulkanGraphicsCommandBuffer.hpp"

#include "Clove/Graphics/Vulkan/VulkanBuffer.hpp"
#include "Clove/Graphics/Vulkan/VulkanCommandBuffer.hpp"
#include "Clove/Graphics/Vulkan/VulkanDescriptorSet.hpp"
#include "Clove/Graphics/Vulkan/VulkanFramebuffer.hpp"
#include "Clove/Graphics/Vulkan/VulkanGraphicsPipelineObject.hpp"
#include "Clove/Graphics/Vulkan/VulkanPipelineObject.hpp"
#include "Clove/Graphics/Vulkan/VulkanImage.hpp"
#include "Clove/Graphics/Vulkan/VulkanMemoryBarrier.hpp"
#include "Clove/Graphics/Vulkan/VulkanRenderPass.hpp"
#include "Clove/Graphics/Vulkan/VulkanResource.hpp"
#include "Clove/Graphics/Vulkan/VulkanShader.hpp"

#include <Clove/Cast.hpp>

namespace garlic::clove {
    namespace {
        VkIndexType getIndexType(IndexType garlicType) {
            switch(garlicType) {
                case IndexType::Uint16:
                    return VK_INDEX_TYPE_UINT16;
                default:
                    CLOVE_ASSERT(false, "{0}: Unkown index type", CLOVE_FUNCTION_NAME);
                    return VK_INDEX_TYPE_UINT16;
            }
        }

        template<typename... Ts>
        struct match : Ts... { using Ts::operator()...; };
        template<typename... Ts>
        match(Ts...) -> match<Ts...>;
    }

    VulkanGraphicsCommandBuffer::VulkanGraphicsCommandBuffer(VkCommandBuffer commandBuffer, QueueFamilyIndices queueFamilyIndices)
        : commandBuffer(commandBuffer)
        , queueFamilyIndices(std::move(queueFamilyIndices)) {
    }

    VulkanGraphicsCommandBuffer::VulkanGraphicsCommandBuffer(VulkanGraphicsCommandBuffer &&other) noexcept = default;

    VulkanGraphicsCommandBuffer &VulkanGraphicsCommandBuffer::operator=(VulkanGraphicsCommandBuffer &&other) noexcept = default;

    VulkanGraphicsCommandBuffer::~VulkanGraphicsCommandBuffer() = default;

    void VulkanGraphicsCommandBuffer::beginRecording(CommandBufferUsage usageFlag) {
        VkCommandBufferBeginInfo beginInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags            = getCommandBufferUsageFlags(usageFlag),
            .pInheritanceInfo = nullptr,
        };

        if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to begin recording command buffer");
        }
    }

    void VulkanGraphicsCommandBuffer::endRecording() {
        if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to end recording command buffer");
        }
    }

    void VulkanGraphicsCommandBuffer::beginRenderPass(GhaRenderPass &renderPass, GhaFramebuffer &frameBuffer, RenderArea const &renderArea, std::span<ClearValue> clearValues) {
        std::vector<VkClearValue> vkClearValues(std::size(clearValues));
        for(uint32_t i{ 0 }; i < clearValues.size(); ++i) {
            std::visit(match{
                           [&](ColourValue const &colour) { vkClearValues[i].color = { colour.r, colour.g, colour.b, colour.a }; },
                           [&](DepthStencilValue const &depthStencil) { vkClearValues[i].depthStencil = { depthStencil.depth, depthStencil.stencil }; },
                       },
                       clearValues[i]);
        }

        VkRenderPassBeginInfo renderPassInfo{
            .sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass  = polyCast<VulkanRenderPass>(&renderPass)->getRenderPass(),
            .framebuffer = polyCast<VulkanFramebuffer>(&frameBuffer)->getFrameBuffer(),
            .renderArea  = {
                .offset = { renderArea.origin.x, renderArea.origin.y },
                .extent = { renderArea.size.x, renderArea.size.y },
            },
            .clearValueCount = static_cast<uint32_t>(std::size(vkClearValues)),
            .pClearValues    = std::data(vkClearValues),
        };

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void VulkanGraphicsCommandBuffer::endRenderPass() {
        vkCmdEndRenderPass(commandBuffer);
    }

    void VulkanGraphicsCommandBuffer::setViewport(vec2i position, vec2ui size) {
        VkViewport const viewport{
            .x        = static_cast<float>(position.x),
            .y        = static_cast<float>(position.y),
            .width    = static_cast<float>(size.x),
            .height   = static_cast<float>(size.y),
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
        };

        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    }

    void VulkanGraphicsCommandBuffer::setScissor(vec2i position, vec2ui size) {
        VkRect2D const scissor{
            .offset = { position.x, position.y },
            .extent = { size.x, size.y },
        };

        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void VulkanGraphicsCommandBuffer::bindPipelineObject(GhaGraphicsPipelineObject &pipelineObject) {
        auto const *pipeline = polyCast<VulkanGraphicsPipelineObject>(&pipelineObject);

        currentLayout = pipeline->getLayout();

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getPipeline());
    }

    void VulkanGraphicsCommandBuffer::bindVertexBuffer(GhaBuffer &vertexBuffer, size_t const offset) {
        VkBuffer buffers[]     = { polyCast<VulkanBuffer>(&vertexBuffer)->getBuffer() };
        VkDeviceSize offsets[] = { offset };

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

    void VulkanGraphicsCommandBuffer::bindIndexBuffer(GhaBuffer &indexBuffer, size_t const offset, IndexType indexType) {
        vkCmdBindIndexBuffer(commandBuffer, polyCast<VulkanBuffer>(&indexBuffer)->getBuffer(), offset, getIndexType(indexType));
    }

    void VulkanGraphicsCommandBuffer::bindDescriptorSet(GhaDescriptorSet &descriptorSet, uint32_t const setNum) {
        VkDescriptorSet sets[] = { polyCast<VulkanDescriptorSet>(&descriptorSet)->getDescriptorSet() };

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, currentLayout, setNum, 1, sets, 0, nullptr);
    }

    void VulkanGraphicsCommandBuffer::pushConstant(GhaShader::Stage const stage, size_t const offset, size_t const size, void const *data) {
        vkCmdPushConstants(commandBuffer, currentLayout, VulkanShader::convertStage(stage), offset, size, data);
    }

    void VulkanGraphicsCommandBuffer::drawIndexed(size_t const indexCount) {
        vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
    }

    void VulkanGraphicsCommandBuffer::bufferMemoryBarrier(GhaBuffer &buffer, BufferMemoryBarrierInfo const &barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage) {
        const uint32_t sourceFamilyIndex{ getQueueFamilyIndex(barrierInfo.sourceQueue, queueFamilyIndices) };
        const uint32_t destinationFamilyIndex{ getQueueFamilyIndex(barrierInfo.destinationQueue, queueFamilyIndices) };

        VkBufferMemoryBarrier barrier{
            .sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
            .pNext               = nullptr,
            .srcAccessMask       = convertAccessFlags(barrierInfo.currentAccess),
            .dstAccessMask       = convertAccessFlags(barrierInfo.newAccess),
            .srcQueueFamilyIndex = sourceFamilyIndex,
            .dstQueueFamilyIndex = destinationFamilyIndex,
            .buffer              = polyCast<VulkanBuffer>(&buffer)->getBuffer(),
            .offset              = 0,
            .size                = VK_WHOLE_SIZE,
        };

        VkPipelineStageFlags const vkSourceStage{ convertStage(sourceStage) };
        VkPipelineStageFlags const vkDestinationStage{ convertStage(destinationStage) };

        vkCmdPipelineBarrier(commandBuffer, vkSourceStage, vkDestinationStage, 0, 0, nullptr, 1, &barrier, 0, nullptr);
    }

    void VulkanGraphicsCommandBuffer::imageMemoryBarrier(GhaImage &image, ImageMemoryBarrierInfo const &barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage) {
        uint32_t const sourceFamilyIndex{ getQueueFamilyIndex(barrierInfo.sourceQueue, queueFamilyIndices) };
        uint32_t const destinationFamilyIndex{ getQueueFamilyIndex(barrierInfo.destinationQueue, queueFamilyIndices) };

        VkImageMemoryBarrier barrier{
            .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext               = nullptr,
            .srcAccessMask       = convertAccessFlags(barrierInfo.currentAccess),
            .dstAccessMask       = convertAccessFlags(barrierInfo.newAccess),
            .oldLayout           = VulkanImage::convertLayout(barrierInfo.currentImageLayout),
            .newLayout           = VulkanImage::convertLayout(barrierInfo.newImageLayout),
            .srcQueueFamilyIndex = sourceFamilyIndex,
            .dstQueueFamilyIndex = destinationFamilyIndex,
            .image               = polyCast<VulkanImage>(&image)->getImage(),
            .subresourceRange    = {
                .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,//TODO: Handle other aspect mask,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = 1,
            },
        };

        VkPipelineStageFlags const vkSourceStage{ convertStage(sourceStage) };
        VkPipelineStageFlags const vkDestinationStage{ convertStage(destinationStage) };

        vkCmdPipelineBarrier(commandBuffer, vkSourceStage, vkDestinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    }

    VkCommandBuffer VulkanGraphicsCommandBuffer::getCommandBuffer() const {
        return commandBuffer;
    }
}