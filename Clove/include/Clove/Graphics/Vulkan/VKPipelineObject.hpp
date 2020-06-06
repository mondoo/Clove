#pragma once

#include "Clove/Graphics/Vulkan/VKShader.hpp"

//TODO: Remove
#include <vulkan/vulkan.h>

namespace clv::gfx {
	//TODO: Remove vulkan references
	struct PiplineObjectDescriptor {
		std::shared_ptr<vk::VKShader> vertexShader;
		std::shared_ptr<vk::VKShader> fragmentShader;
	};
}

namespace clv::gfx::vk {
	class VKPipelineObject {
		//VARIABLES
	private:
		VkDevice device = VK_NULL_HANDLE;

		VkPipeline pipeline = VK_NULL_HANDLE;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKPipelineObject(VkDevice device, PiplineObjectDescriptor descriptor);

		~VKPipelineObject();
	};
}