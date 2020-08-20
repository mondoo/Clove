#pragma once

#include "Clove/Graphics/RenderPass.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKRenderPass : public RenderPass {
        //VARIABLES
    private:
        DevicePointer device;

        VkRenderPass renderPass{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        VKRenderPass() = delete;
        VKRenderPass(DevicePointer device, Descriptor descriptor);

        VKRenderPass(const VKRenderPass& other) = delete;
        VKRenderPass(VKRenderPass&& other) noexcept;

        VKRenderPass& operator=(const VKRenderPass& other) = delete;
        VKRenderPass& operator=(VKRenderPass&& other) noexcept;

        ~VKRenderPass();

        VkRenderPass getRenderPass() const;
    };
}