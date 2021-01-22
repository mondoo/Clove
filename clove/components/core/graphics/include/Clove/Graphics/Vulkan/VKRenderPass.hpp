#pragma once

#include "Clove/Graphics/GhaRenderPass.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

#include <vulkan/vulkan.h>

namespace garlic::clove {
    class VKRenderPass : public GhaRenderPass {
        //VARIABLES
    private:
        DevicePointer device;

        VkRenderPass renderPass{ VK_NULL_HANDLE };

        //FUNCTIONS
    public:
        VKRenderPass() = delete;
        VKRenderPass(DevicePointer device, VkRenderPass renderPass);

        VKRenderPass(VKRenderPass const &other) = delete;
        VKRenderPass(VKRenderPass &&other) noexcept;

        VKRenderPass &operator=(VKRenderPass const &other) = delete;
        VKRenderPass &operator=(VKRenderPass &&other) noexcept;

        ~VKRenderPass();

        VkRenderPass getRenderPass() const;
    };
}