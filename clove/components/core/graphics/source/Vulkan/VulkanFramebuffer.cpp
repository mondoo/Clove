#include "Clove/Graphics/Vulkan/VulkanFramebuffer.hpp"

#include "Clove/Graphics/Vulkan/VulkanImageView.hpp"
#include "Clove/Graphics/Vulkan/VulkanRenderPass.hpp"

#include <Clove/Cast.hpp>

namespace garlic::clove {
    VulkanFramebuffer::VulkanFramebuffer(DevicePointer device, VkFramebuffer framebuffer)
        : device{ std::move(device) }
        , framebuffer{ framebuffer } {
    }

    VulkanFramebuffer::VulkanFramebuffer(VulkanFramebuffer &&other) noexcept = default;

    VulkanFramebuffer &VulkanFramebuffer::operator=(VulkanFramebuffer &&other) noexcept = default;

    VulkanFramebuffer::~VulkanFramebuffer() {
        vkDestroyFramebuffer(device.get(), framebuffer, nullptr);
    }

    VkFramebuffer VulkanFramebuffer::getFrameBuffer() const {
        return framebuffer;
    }
}