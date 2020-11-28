#include "Clove/Graphics/Vulkan/VKFramebuffer.hpp"

#include "Clove/Graphics/Vulkan/VKImageView.hpp"
#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"

#include <Clove/Cast.hpp>

namespace garlic::clove {
    VKFramebuffer::VKFramebuffer(DevicePointer device, VkFramebuffer framebuffer)
        : device{ std::move(device) }
        , framebuffer{ framebuffer } {
    }

    VKFramebuffer::VKFramebuffer(VKFramebuffer &&other) noexcept = default;

    VKFramebuffer &VKFramebuffer::operator=(VKFramebuffer &&other) noexcept = default;

    VKFramebuffer::~VKFramebuffer() {
        vkDestroyFramebuffer(device.get(), framebuffer, nullptr);
    }

    VkFramebuffer VKFramebuffer::getFrameBuffer() const {
        return framebuffer;
    }
}