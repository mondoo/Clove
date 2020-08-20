#include "Clove/Graphics/Vulkan/VKFramebuffer.hpp"

#include "Clove/Graphics/Vulkan/VKImageView.hpp"
#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"
#include "Clove/Utils/Cast.hpp"

namespace clv::gfx::vk {
    VKFramebuffer::VKFramebuffer(DevicePointer device, Descriptor descriptor)
        : device(std::move(device)) {
        std::vector<VkImageView> attachments;
        attachments.reserve(std::size(descriptor.attachments));
        for(auto& attachment : descriptor.attachments) {
            const VKImageView* vkImageView = polyCast<VKImageView>(attachment.get());
            attachments.push_back(vkImageView->getImageView());
        }

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.pNext           = nullptr;
        framebufferInfo.flags           = 0;
        framebufferInfo.renderPass      = polyCast<VKRenderPass>(descriptor.renderPass.get())->getRenderPass();
        framebufferInfo.attachmentCount = std::size(attachments);
        framebufferInfo.pAttachments    = std::data(attachments);
        framebufferInfo.width           = descriptor.width;
        framebufferInfo.height          = descriptor.height;
        framebufferInfo.layers          = 1;

        if(vkCreateFramebuffer(this->device.get(), &framebufferInfo, nullptr, &framebuffer) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create framebuffer");
        }
    }

    VKFramebuffer::VKFramebuffer(VKFramebuffer&& other) noexcept = default;

    VKFramebuffer& VKFramebuffer::operator=(VKFramebuffer&& other) noexcept = default;

    VKFramebuffer::~VKFramebuffer() {
        vkDestroyFramebuffer(device.get(), framebuffer, nullptr);
    }

    VkFramebuffer VKFramebuffer::getFrameBuffer() const {
        return framebuffer;
    }
}