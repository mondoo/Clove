#include "Clove/Graphics/Vulkan/VKFramebuffer.hpp"

#include "Clove/Graphics/Vulkan/VKImageView.hpp"
#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"

#include <Clove/Cast.hpp>

namespace garlic::clove {
    VKFramebuffer::VKFramebuffer(DevicePointer device, Descriptor descriptor)
        : device(std::move(device)) {
        std::vector<VkImageView> attachments;
        attachments.reserve(std::size(descriptor.attachments));
        for(auto &attachment : descriptor.attachments) {
            VKImageView const *vkImageView = polyCast<VKImageView>(attachment.get());
            attachments.push_back(vkImageView->getImageView());
        }

        VkFramebufferCreateInfo framebufferInfo{
            .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext           = nullptr,
            .flags           = 0,
            .renderPass      = polyCast<VKRenderPass>(descriptor.renderPass.get())->getRenderPass(),
            .attachmentCount = static_cast<uint32_t>(std::size(attachments)),
            .pAttachments    = std::data(attachments),
            .width           = descriptor.width,
            .height          = descriptor.height,
            .layers          = 1,
        };

        if(vkCreateFramebuffer(this->device.get(), &framebufferInfo, nullptr, &framebuffer) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, LogLevel::Error, "Failed to create framebuffer");
        }
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