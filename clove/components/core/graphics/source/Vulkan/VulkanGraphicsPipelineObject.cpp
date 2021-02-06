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
}