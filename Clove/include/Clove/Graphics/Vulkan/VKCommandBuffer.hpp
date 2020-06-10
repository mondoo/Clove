#pragma once

//TODO: Move to cpp
#include <vulkan/vulkan.h>
#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"
#include "Clove/Graphics/Vulkan/VKFramebuffer.hpp"
#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"

namespace clv::gfx{
	//TODO: Need some sort of generic area?
	struct RenderArea{
		mth::vec2i origin;
		mth::vec2ui size;
	};
}

namespace clv::gfx::vk {
	class VKCommandBuffer {
		//VARIABLES
	private:
		VkCommandBuffer commandBuffer = VK_NULL_HANDLE;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKCommandBuffer(VkCommandBuffer commandBuffer);

		void beginRecording();

		void beginRenderPass(VKRenderPass& renderPass, VKFramebuffer& frameBuffer, const RenderArea& renderArea, const mth::vec4f& clearColour);

		void bindPipelineObject(VKPipelineObject& pipelineObject);
		void drawIndexed(); //TEMP: Not using buffers right now, this is just for testing

		void endRenderPass();

		void endRecording();
	};
}