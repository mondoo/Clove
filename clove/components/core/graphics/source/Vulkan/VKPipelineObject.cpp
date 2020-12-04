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
    VKPipelineObject::VKPipelineObject(DevicePointer device, VkPipeline pipeline, VkPipelineLayout pipelineLayout)
        : device{ std::move(device) }
        , pipeline{ pipeline }
        , pipelineLayout{ pipelineLayout } {
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
                CLOVE_ASSERT(false, "{0}: Unkown pipeline stage", CLOVE_FUNCTION_NAME);
                return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        }
    }
}