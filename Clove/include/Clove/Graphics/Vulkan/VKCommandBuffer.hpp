#pragma once

//TODO: Move to cpp
#include <vulkan/vulkan.h>
#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"
#include "Clove/Graphics/Vulkan/VKFramebuffer.hpp"
#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"
#include "Clove/Graphics/Vulkan/VKBuffer.hpp"

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

		void beginRenderPass(VKRenderPass& renderPass, VKFramebuffer& frameBuffer, const RenderArea& renderArea, const mth::vec4f& clearColour);

		void bindPipelineObject(VKPipelineObject& pipelineObject);
        void bindVertexBuffer(VKBuffer& vertexBuffer);
        void bindIndexBuffer(VKBuffer& indexBuffer, IndexType indexType);
        void drawIndexed(const size_t indexCount);

		void endRenderPass();

		void endRecording();

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

        void copyBuffer(VKBuffer& source, const size_t sourceOffset, VKBuffer& destination, const size_t destinationOffset, const size_t sizeBytes);

        void endRecording();

        VkCommandBuffer getCommandBuffer() const;
	};
}