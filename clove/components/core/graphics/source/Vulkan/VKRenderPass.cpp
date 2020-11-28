#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"

#include "Clove/Graphics/Vulkan/VKImage.hpp"
#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"

#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    VKRenderPass::VKRenderPass(DevicePointer device, VkRenderPass renderPass)
        : device{ std::move(device) }
        , renderPass{ renderPass } {
    }

    VKRenderPass::VKRenderPass(VKRenderPass &&other) noexcept = default;

    VKRenderPass &VKRenderPass::operator=(VKRenderPass &&other) noexcept = default;

    VKRenderPass::~VKRenderPass() {
        vkDestroyRenderPass(device.get(), renderPass, nullptr);
    }

    VkRenderPass VKRenderPass::getRenderPass() const {
        return renderPass;
    }
}