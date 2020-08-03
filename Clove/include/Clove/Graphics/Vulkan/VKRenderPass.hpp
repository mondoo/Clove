#pragma once

#include "Clove/Graphics/RenderPass.hpp"

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    class VKRenderPass : public RenderPass {
        //VARIABLES
    private:
        VkDevice device = VK_NULL_HANDLE;

        VkRenderPass renderPass = VK_NULL_HANDLE;

        //FUNCTIONS
    public:
        VKRenderPass() = delete;
        VKRenderPass(VkDevice device, Descriptor descriptor);

        VKRenderPass(const VKRenderPass& other) = delete;
        VKRenderPass(VKRenderPass&& other) noexcept;

        VKRenderPass& operator=(const VKRenderPass& other) = delete;
        VKRenderPass& operator=(VKRenderPass&& other) noexcept;

        ~VKRenderPass();

        VkRenderPass getRenderPass() const;
    };
}