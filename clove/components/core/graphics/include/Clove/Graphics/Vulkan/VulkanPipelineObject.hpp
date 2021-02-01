#pragma once

#include "Clove/Graphics/GhaPipelineObject.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    class VulkanPipelineObject : public GhaPipelineObject {
        //VARIABLES
    private:
        DevicePointer device;

        VkPipeline pipeline{ VK_NULL_HANDLE };
        VkPipelineLayout pipelineLayout{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        VulkanPipelineObject() = delete;
        VulkanPipelineObject(DevicePointer device, VkPipeline pipeline, VkPipelineLayout pipelineLayout);

        VulkanPipelineObject(VulkanPipelineObject const &other) = delete;
        VulkanPipelineObject(VulkanPipelineObject &&other) noexcept;

        VulkanPipelineObject &operator=(VulkanPipelineObject const &other) = delete;
        VulkanPipelineObject &operator=(VulkanPipelineObject &&other) noexcept;

        ~VulkanPipelineObject();

        VkPipeline getPipeline() const;
        VkPipelineLayout getLayout() const;

        static VkPipelineStageFlags convertStage(Stage garlicStage);
    };
}