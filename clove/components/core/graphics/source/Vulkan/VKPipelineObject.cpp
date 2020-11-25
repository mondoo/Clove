#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"

#include "Clove/Graphics/Vulkan/VKDescriptorSetLayout.hpp"
#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"
#include "Clove/Graphics/Vulkan/VKShader.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include <Clove/Cast.hpp>
#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>
#include <array>

namespace garlic::clove {
    static VkFormat convertAttributeFormat(VertexAttributeFormat garlicFormat) {
        switch(garlicFormat) {
            case VertexAttributeFormat::R32_SFLOAT:
                return VK_FORMAT_R32_SFLOAT;
            case VertexAttributeFormat::R32G32_SFLOAT:
                return VK_FORMAT_R32G32_SFLOAT;
            case VertexAttributeFormat::R32G32B32_SFLOAT:
                return VK_FORMAT_R32G32B32_SFLOAT;
            case VertexAttributeFormat::R32G32B32A32_SFLOAT:
                return VK_FORMAT_R32G32B32A32_SFLOAT;
            case VertexAttributeFormat::R32G32B32A32_SINT:
                return VK_FORMAT_R32G32B32A32_SINT;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown format passed", GARLIC_FUNCTION_NAME_PRETTY);
                return VK_FORMAT_UNDEFINED;
        }
    }

    static VkVertexInputBindingDescription getBindingDescription(VertexInputBindingDescriptor const &garlicDescriptor) {
        VkVertexInputBindingDescription description{
            .binding   = 0,
            .stride    = garlicDescriptor.stride,
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
        };
        return description;
    }

    static std::vector<VkVertexInputAttributeDescription> getAttributes(std::vector<VertexAttributeDescriptor> const &garlicAttributes) {
        std::vector<VkVertexInputAttributeDescription> attributes(std::size(garlicAttributes));
        for(size_t i = 0; i < std::size(attributes); ++i) {
            attributes[i].binding  = 0,
            attributes[i].location = garlicAttributes[i].location;
            attributes[i].format   = convertAttributeFormat(garlicAttributes[i].format);
            attributes[i].offset   = garlicAttributes[i].offset;
        }
        return attributes;
    }

    static std::vector<VkDescriptorSetLayout> getDescriptorSetLayouts(std::vector<VKDescriptorSetLayout *> const &garlicLayouts) {
        std::vector<VkDescriptorSetLayout> layouts(std::size(garlicLayouts));
        for(size_t i = 0; i < std::size(layouts); ++i) {
            layouts[i] = garlicLayouts[i]->getLayout();
        }
        return layouts;
    }

    VKPipelineObject::VKPipelineObject(DevicePointer device, Descriptor descriptor)
        : device(std::move(device)) {
        std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages;

        VkVertexInputBindingDescription inputBindingDescription         = getBindingDescription(descriptor.vertexInput);
        std::vector<VkVertexInputAttributeDescription> vertexAttributes = getAttributes(descriptor.vertexAttributes);

        //Descriptor set layouts
        std::vector<VKDescriptorSetLayout *> castedLayouts(std::size(descriptor.descriptorSetLayouts));
        for(size_t i = 0; i < std::size(castedLayouts); ++i) {
            castedLayouts[i] = polyCast<VKDescriptorSetLayout>(descriptor.descriptorSetLayouts[i].get());
        }
        std::vector<VkDescriptorSetLayout> descriptorLayouts = getDescriptorSetLayouts(castedLayouts);

        //Push constants
        std::vector<VkPushConstantRange> vkPushConstantRanges(std::size(descriptor.pushConstants));
        for(size_t i = 0; i < std::size(vkPushConstantRanges); ++i) {
            vkPushConstantRanges[i].stageFlags = VKShader::convertStage(descriptor.pushConstants[i].stage);
            vkPushConstantRanges[i].offset     = descriptor.pushConstants[i].offset;
            vkPushConstantRanges[i].size       = descriptor.pushConstants[i].size;
        }

        //Vertex shader
        VkPipelineShaderStageCreateInfo vertShaderStageInfo{
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage  = VK_SHADER_STAGE_VERTEX_BIT,
            .module = polyCast<VKShader>(descriptor.vertexShader.get())->getModule(),
            .pName  = "main",
        };
        shaderStages[0] = vertShaderStageInfo;

        //Fragment shader
        VkPipelineShaderStageCreateInfo fragShaderStageInfo{
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage  = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = polyCast<VKShader>(descriptor.fragmentShader.get())->getModule(),
            .pName  = "main",
        };
        shaderStages[1] = fragShaderStageInfo;

        //Vertex input
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{
            .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .vertexBindingDescriptionCount   = 1,
            .pVertexBindingDescriptions      = &inputBindingDescription,
            .vertexAttributeDescriptionCount = static_cast<uint32_t>(std::size(vertexAttributes)),
            .pVertexAttributeDescriptions    = std::data(vertexAttributes),
        };

        //Input assembly
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE,
        };

        //Viewport + Scissor
        //TODO: Dynamic viewport / scissor rect
        VkViewport viewport{
            .x        = static_cast<float>(descriptor.viewportDescriptor.position.x),
            .y        = static_cast<float>(descriptor.viewportDescriptor.position.y),
            .width    = static_cast<float>(descriptor.viewportDescriptor.size.x),
            .height   = static_cast<float>(descriptor.viewportDescriptor.size.y),
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
        };

        VkRect2D scissor{
            .offset = { descriptor.scissorDescriptor.position.x, descriptor.scissorDescriptor.position.y },
            .extent = { descriptor.scissorDescriptor.size.x, descriptor.scissorDescriptor.size.y },
        };

        VkPipelineViewportStateCreateInfo viewportState{
            .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount = 1,
            .pViewports    = &viewport,
            .scissorCount  = 1,
            .pScissors     = &scissor,
        };

        //Rasteriser
        VkPipelineRasterizationStateCreateInfo rasterizer{
            .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .depthClampEnable        = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,
            .polygonMode             = VK_POLYGON_MODE_FILL,
            .cullMode                = VK_CULL_MODE_BACK_BIT,
            .frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE,
            .depthBiasEnable         = VK_FALSE,
            .lineWidth               = 1.0f,//Any thicker requires the 'wideLines' feature
        };

        //Multisampling
        VkPipelineMultisampleStateCreateInfo multisampling{
            .sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .sampleShadingEnable  = VK_FALSE,
        };

        //Depth / Stencil
        VkPipelineDepthStencilStateCreateInfo depthStencil{
            .sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .pNext                 = nullptr,
            .flags                 = 0,
            .depthTestEnable       = static_cast<VkBool32>(descriptor.depthState.depthTest ? VK_TRUE : VK_FALSE),
            .depthWriteEnable      = static_cast<VkBool32>(descriptor.depthState.depthWrite ? VK_TRUE : VK_FALSE),
            .depthCompareOp        = VK_COMPARE_OP_LESS,
            .depthBoundsTestEnable = VK_FALSE,
            .stencilTestEnable     = VK_FALSE,
            .front                 = {},
            .back                  = {},
            .minDepthBounds        = 0.0f,
            .maxDepthBounds        = 1.0f,
        };

        //Colour blending
        VkPipelineColorBlendAttachmentState colourBlendAttachment{
            .blendEnable         = static_cast<VkBool32>(descriptor.enableBlending ? VK_TRUE : VK_FALSE),
            .srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
            .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            .colorBlendOp        = VK_BLEND_OP_ADD,
            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
            .alphaBlendOp        = VK_BLEND_OP_ADD,
            .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
        };

        VkPipelineColorBlendStateCreateInfo colorBlending{
            .sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable   = VK_FALSE,
            .logicOp         = VK_LOGIC_OP_COPY,
            .attachmentCount = 1,
            .pAttachments    = &colourBlendAttachment,
            .blendConstants  = { 0.0f, 0.0f, 0.0f, 0.0f },
        };

        //Pipeline Layout
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount         = static_cast<uint32_t>(std::size(descriptorLayouts)),
            .pSetLayouts            = std::data(descriptorLayouts),
            .pushConstantRangeCount = static_cast<uint32_t>(std::size(vkPushConstantRanges)),
            .pPushConstantRanges    = std::data(vkPushConstantRanges),
        };

        if(vkCreatePipelineLayout(this->device.get(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            GARLIC_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to create pipeline layout");
            return;
        }

        //Pipeline
        VkGraphicsPipelineCreateInfo pipelineInfo{
            .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext               = nullptr,
            .stageCount          = std::size(shaderStages),
            .pStages             = std::data(shaderStages),
            .pVertexInputState   = &vertexInputInfo,
            .pInputAssemblyState = &inputAssembly,
            .pTessellationState  = nullptr,
            .pViewportState      = &viewportState,
            .pRasterizationState = &rasterizer,
            .pMultisampleState   = &multisampling,
            .pDepthStencilState  = &depthStencil,
            .pColorBlendState    = &colorBlending,
            .pDynamicState       = nullptr,//TODO
            .layout              = pipelineLayout,
            .renderPass          = polyCast<VKRenderPass>(descriptor.renderPass.get())->getRenderPass(),
            .subpass             = 0,//The subpass of the renderpass that'll use this pipeline
            .basePipelineHandle  = VK_NULL_HANDLE,
            .basePipelineIndex   = -1,
        };

        if(vkCreateGraphicsPipelines(this->device.get(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
            GARLIC_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to create graphics pipeline");
        }
    }

    VKPipelineObject::VKPipelineObject(VKPipelineObject &&other) noexcept = default;

    VKPipelineObject &VKPipelineObject::operator=(VKPipelineObject &&other) noexcept = default;

    VKPipelineObject::~VKPipelineObject() {
        vkDestroyPipeline(device.get(), pipeline, nullptr);
        vkDestroyPipelineLayout(device.get(), pipelineLayout, nullptr);
    }

    VkPipeline VKPipelineObject::getPipeline() const {
        return pipeline;
    }

    VkPipelineLayout VKPipelineObject::getLayout() const {
        return pipelineLayout;
    }

    VkPipelineStageFlags VKPipelineObject::convertStage(Stage garlicStage) {
        switch(garlicStage) {
            case Stage::Top:
                return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            case Stage::Transfer:
                return VK_PIPELINE_STAGE_TRANSFER_BIT;
            case Stage::PixelShader:
                return VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            case Stage::EarlyPixelTest:
                return VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            case Stage::ColourAttachmentOutput:
                return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown pipeline stage", GARLIC_FUNCTION_NAME);
                return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        }
    }
}