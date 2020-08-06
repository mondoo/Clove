#pragma once

#include "Clove/Graphics/PipelineObject.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKPipelineObject : public PipelineObject {
        //VARIABLES
    private:
        VkDevice device = VK_NULL_HANDLE;

        VkPipeline pipeline             = VK_NULL_HANDLE;
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;

        //FUNCTIONS
    public:
        VKPipelineObject() = delete;
        VKPipelineObject(VkDevice device, Descriptor descriptor);

        VKPipelineObject(const VKPipelineObject& other) = delete;
        VKPipelineObject(VKPipelineObject&& other) noexcept;

        VKPipelineObject& operator=(const VKPipelineObject& other) = delete;
        VKPipelineObject& operator=(VKPipelineObject&& other) noexcept;

        ~VKPipelineObject();

        VkPipeline getPipeline() const;
        VkPipelineLayout getLayout() const;
    };
}