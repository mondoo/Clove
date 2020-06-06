#pragma once

#include "Clove/Graphics/Vulkan/VKShader.hpp"

//TODO: Remove
#include <vulkan/vulkan.h>

namespace clv::gfx {
	enum class ElementState {
		Static,
		Dynamic
	};

	struct ViewportDescriptor {
		ElementState state{ ElementState::Static };
		mth::vec2f position{ 0.0f, 0.0f };
		mth::vec2f size{ 0.0f, 0.0f };
	};

	struct ScissorDescriptor {
		ElementState state{ ElementState::Static };
		mth::vec2f position{ 0.0f, 0.0f };
		mth::vec2f size{ 0.0f, 0.0f };
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