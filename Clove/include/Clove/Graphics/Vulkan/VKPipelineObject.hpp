#pragma once

//TODO: Remove
#include "Clove/Graphics/Vulkan/VKShader.hpp"
#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"
#include <vulkan/vulkan.h>

namespace clv::gfx {
	enum class ElementState {
		Static,
		Dynamic
	};

	struct ViewportDescriptor {
		ElementState state{ ElementState::Static };
		mth::vec2i position{ 0.0f, 0.0f };
		mth::vec2ui size{ 0.0f, 0.0f };
	};

	struct ScissorDescriptor {
		ElementState state{ ElementState::Static };
		mth::vec2i position{ 0.0f, 0.0f };
		mth::vec2ui size{ 0.0f, 0.0f };
	};

	struct RasteriserDescriptor {
	};

	//TODO: Remove vulkan references
	struct PiplineObjectDescriptor {
		std::shared_ptr<vk::VKShader> vertexShader;
		std::shared_ptr<vk::VKShader> fragmentShader;

		ViewportDescriptor viewportDescriptor;
		ScissorDescriptor scissorDescriptor;

		RasteriserDescriptor rasteriserDescriptor;

		std::shared_ptr<vk::VKRenderPass> renderPass;
	};
}

namespace clv::gfx::vk {
	class VKPipelineObject {
		//VARIABLES
	private:
		VkDevice device = VK_NULL_HANDLE;

		VkPipeline pipeline				= VK_NULL_HANDLE;
		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKPipelineObject(VkDevice device, PiplineObjectDescriptor descriptor);

		~VKPipelineObject();
	};
}