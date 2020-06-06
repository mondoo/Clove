#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"

namespace clv::gfx::vk {
	VKPipelineObject::VKPipelineObject(VkDevice device, PiplineObjectDescriptor descriptor)
		: device(device) {
		std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages;

		//Vertex shader
		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = descriptor.vertexShader->getModule();
		vertShaderStageInfo.pName  = "main";
		shaderStages[0]			   = vertShaderStageInfo;

		//Fragment shader
		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = descriptor.fragmentShader->getModule();
		fragShaderStageInfo.pName  = "main";
		shaderStages[1]			   = fragShaderStageInfo;

		//Vertex input
		//TEMP: vertex data is currently hard coded in the shaders, so this is emptier than usual
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType							= VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount	= 0;
		vertexInputInfo.pVertexBindingDescriptions		= nullptr;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions	= nullptr;

		//Input assembly
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType					 = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology				 = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		//Viewport + Scissor
		//TODO: Dynamic viewport / scissor rect
		VkViewport viewport{};
		viewport.x		  = descriptor.viewportDescriptor.position.x;
		viewport.y		  = descriptor.viewportDescriptor.position.y;
		viewport.width	  = descriptor.viewportDescriptor.size.x;
		viewport.height	  = descriptor.viewportDescriptor.size.y;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { descriptor.scissorDescriptor.position.x, descriptor.scissorDescriptor.position.y };
		scissor.extent = { descriptor.scissorDescriptor.size.x, descriptor.scissorDescriptor.size.y };

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType			= VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports	= &viewport;
		viewportState.scissorCount	= 1;
		viewportState.pScissors		= &scissor;

		//Rasteriser
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType				   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable		   = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode			   = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth			   = 1.0f;//Any thicker requires the 'wideLines' feature
		rasterizer.cullMode				   = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace			   = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable		   = VK_FALSE;

		//Multisampling
		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType				   = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable  = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		//Depth / Stencil
		//TODO: depth / stencil

		//Colour blending
		VkPipelineColorBlendAttachmentState colourBlendAttachment{};
		colourBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colourBlendAttachment.blendEnable	 = VK_FALSE;//TODO: Enable blending

		//Pipeline Layout
		//TODO: pipeline layout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType				  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount		  = 0;
		pipelineLayoutInfo.pSetLayouts			  = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges	  = nullptr;

		if(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create pipeline layout");
			return;
		}

		//Pipeline
		//TODO: pipeline
	}

	VKPipelineObject::~VKPipelineObject() {
		vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	}
}