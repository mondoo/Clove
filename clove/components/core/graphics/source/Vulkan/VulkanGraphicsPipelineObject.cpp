#include "Clove/Graphics/Vulkan/VulkanGraphicsPipelineObject.hpp"

#include "Clove/Graphics/Vulkan/VulkanDescriptorSetLayout.hpp"
#include "Clove/Graphics/Vulkan/VulkanRenderPass.hpp"
#include "Clove/Graphics/Vulkan/VulkanShader.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include <Clove/Cast.hpp>
#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>
#include <array>

namespace garlic::clove {
    VulkanGraphicsPipelineObject::VulkanGraphicsPipelineObject(DevicePointer device, VkPipeline pipeline, VkPipelineLayout pipelineLayout)
        : device{ std::move(device) }
        , pipeline{ pipeline }
        , pipelineLayout{ pipelineLayout } {
    }

    VulkanGraphicsPipelineObject::VulkanGraphicsPipelineObject(VulkanGraphicsPipelineObject &&other) noexcept = default;

    VulkanGraphicsPipelineObject &VulkanGraphicsPipelineObject::operator=(VulkanGraphicsPipelineObject &&other) noexcept = default;

    VulkanGraphicsPipelineObject::~VulkanGraphicsPipelineObject() {
        vkDestroyPipeline(device.get(), pipeline, nullptr);
        vkDestroyPipelineLayout(device.get(), pipelineLayout, nullptr);
    }

    VkPipeline VulkanGraphicsPipelineObject::getPipeline() const {
        return pipeline;
    }

    VkPipelineLayout VulkanGraphicsPipelineObject::getLayout() const {
        return pipelineLayout;
    }

    VkPipelineStageFlags VulkanGraphicsPipelineObject::convertStage(Stage garlicStage) {
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
                CLOVE_ASSERT(false, "{0}: Unkown pipeline stage", CLOVE_FUNCTION_NAME);
                return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        }
    }
}