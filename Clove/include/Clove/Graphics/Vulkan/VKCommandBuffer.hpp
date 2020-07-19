#pragma once

//TODO: Move to cpp
#include <vulkan/vulkan.h>
#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"
#include "Clove/Graphics/Vulkan/VKFramebuffer.hpp"
#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"
#include "Clove/Graphics/Vulkan/VKBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKImage.hpp"
#include "Clove/Graphics/Vulkan/VKDescriptorSet.hpp"

namespace clv::gfx{
	//TODO: Need some sort of generic area?
    struct RenderArea {
        mth::vec2i origin;
        mth::vec2ui size;
    };

    enum class CommandBufferUsage {
        Default,
        OneTimeSubmit,
    };

    enum class IndexType{
        Uint16
    };
}

namespace clv::gfx::vk {
	//TODO: Move out into different files

	class VKGraphicsCommandBuffer {
		//VARIABLES
	private:
		VkCommandBuffer commandBuffer = VK_NULL_HANDLE;

		//FUNCTIONS
	public:
		//TODO: Ctors
        VKGraphicsCommandBuffer(VkCommandBuffer commandBuffer);

		void beginRecording(CommandBufferUsage usageFlag);
        void endRecording();

		void beginRenderPass(VKRenderPass& renderPass, VKFramebuffer& frameBuffer, const RenderArea& renderArea, const mth::vec4f& clearColour);
        void endRenderPass();

		void bindPipelineObject(VKPipelineObject& pipelineObject);
        void bindVertexBuffer(VKBuffer& vertexBuffer);
        void bindIndexBuffer(VKBuffer& indexBuffer, IndexType indexType);
        void bindDescriptorSet(VKDescriptorSet& descriptorSet, const VKPipelineObject& pipeline);

        void drawIndexed(const size_t indexCount);

        void transitionImageLayout(VKImage& image, ImageLayout previousLayout, ImageLayout newLayout);

		VkCommandBuffer getCommandBuffer() const;
	};

	class VKTransferCommandBuffer{
        //VARIABLES
    private:
        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;

        //FUNCTIONS
    public:
        //TODO: Ctors
        VKTransferCommandBuffer(VkCommandBuffer commandBuffer);

        void beginRecording(CommandBufferUsage usageFlag);
        void endRecording();

        void copyBufferToBuffer(VKBuffer& source, const size_t sourceOffset, VKBuffer& destination, const size_t destinationOffset, const size_t sizeBytes);
        void copyBufferToImage(VKBuffer& source, const size_t sourceOffset, VKImage& destination, ImageLayout destinationLayout, const mth::vec3i& destinationOffset, const mth::vec3ui& destinationExtent);

        void transitionImageLayout(VKImage& image, ImageLayout previousLayout, ImageLayout newLayout);

        VkCommandBuffer getCommandBuffer() const;
	};
}