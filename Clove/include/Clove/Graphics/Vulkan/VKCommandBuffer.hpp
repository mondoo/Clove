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
		void drawIndexed(const size_t vertexCount);

		void endRenderPass();

		void endRecording();

		VkCommandBuffer getCommandBuffer() const;
	};
}