#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"

namespace clv::gfx::vk {
	VKPipelineObject::VKPipelineObject(VkDevice device, PiplineObjectDescriptor descriptor)
		: device(device) {
		//Vertex shader
		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = descriptor.vertexShader->getModule();
		vertShaderStageInfo.pName  = "main";

		//Fragment shader
		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = descriptor.fragmentShader->getModule();
		fragShaderStageInfo.pName  = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = {
			vertShaderStageInfo,
			fragShaderStageInfo,
		};
	}

	VKPipelineObject::~VKPipelineObject() {
	}
}