#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"

#include "Clove/Graphics/Vulkan/VulkanHelpers.hpp"

namespace clv::gfx::vk {
    static VkVertexInputBindingDescription getBindingDescription(const VertexInputBindingDescriptor& garlicDescriptor) {
        VkVertexInputBindingDescription description{};
        description.binding   = garlicDescriptor.binding;
        description.stride    = garlicDescriptor.stride;
        description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return description;
    }

    static std::vector<VkVertexInputAttributeDescription> getAttributes(const std::vector<VertexAttributeDescriptor>& garlicAttributes) {
        std::vector<VkVertexInputAttributeDescription> attributes(std::size(garlicAttributes));
        for(size_t i = 0; i < std::size(attributes); ++i) {
            attributes[i].binding  = garlicAttributes[i].binding;
            attributes[i].location = garlicAttributes[i].location;
            attributes[i].format   = convertAttributeFormat(garlicAttributes[i].format);
            attributes[i].offset   = garlicAttributes[i].offset;
		}
		return attributes;
    }

	static std::vector<VkDescriptorSetLayout> getDescriptorSetLayouts(const std::vector<std::shared_ptr<VKDescriptorSetLayout>>& garlicLayouts) {
        std::vector<VkDescriptorSetLayout> layouts(std::size(garlicLayouts));
        for(size_t i = 0; i < std::size(layouts); ++i) {
            layouts[i] = garlicLayouts[i]->getLayout();
		}
        return layouts;
    }

	VKPipelineObject::VKPipelineObject(VkDevice device, PiplineObjectDescriptor descriptor)
		: device(device) {
		std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages;

		VkVertexInputBindingDescription inputBindingDescription         = getBindingDescription(descriptor.vertexInput);
        std::vector<VkVertexInputAttributeDescription> vertexAttributes = getAttributes(descriptor.vertexAttributes);

		std::vector<VkDescriptorSetLayout> descriptorLayouts = getDescriptorSetLayouts(descriptor.descriptorSetLayouts);

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
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType							= VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount	= 1;
        vertexInputInfo.pVertexBindingDescriptions      = &inputBindingDescription;
		vertexInputInfo.vertexAttributeDescriptionCount = std::size(vertexAttributes);
		vertexInputInfo.pVertexAttributeDescriptions	= std::data(vertexAttributes);

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
        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.pNext                 = nullptr;
        depthStencil.flags                 = 0;
        depthStencil.depthTestEnable       = VK_TRUE;
        depthStencil.depthWriteEnable      = VK_TRUE;
        depthStencil.depthCompareOp        = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.stencilTestEnable     = VK_FALSE;
        depthStencil.front                 = {};
        depthStencil.back                  = {};
        depthStencil.minDepthBounds        = 0.0f;
        depthStencil.maxDepthBounds        = 1.0f;

		//Colour blending
		VkPipelineColorBlendAttachmentState colourBlendAttachment{};
		colourBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colourBlendAttachment.blendEnable	 = VK_FALSE;//TODO: Enable blending

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType				= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable		= VK_FALSE;
		colorBlending.logicOp			= VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount	= 1;
		colorBlending.pAttachments		= &colourBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		//Pipeline Layout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType				  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount		  = std::size(descriptorLayouts);
		pipelineLayoutInfo.pSetLayouts			  = std::data(descriptorLayouts);
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges	  = nullptr;

		if(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create pipeline layout");
			return;
		}

		//Pipeline
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType				 = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.pNext               = nullptr;
		pipelineInfo.stageCount			 = std::size(shaderStages);
		pipelineInfo.pStages			 = std::data(shaderStages);
		pipelineInfo.pVertexInputState	 = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pTessellationState  = nullptr;
		pipelineInfo.pViewportState		 = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState	 = &multisampling;
		pipelineInfo.pDepthStencilState	 = &depthStencil;
		pipelineInfo.pColorBlendState	 = &colorBlending;
		pipelineInfo.pDynamicState		 = nullptr;//TODO
		pipelineInfo.layout				 = pipelineLayout;
		pipelineInfo.renderPass			 = descriptor.renderPass->getRenderPass();
		pipelineInfo.subpass			 = 0;//The subpass of the renderpass that'll use this pipeline
		pipelineInfo.basePipelineHandle	 = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex	 = -1;

		if(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create graphics pipeline");
		}
	}

	VKPipelineObject::~VKPipelineObject() {
		vkDestroyPipeline(device, pipeline, nullptr);
		vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	}
}