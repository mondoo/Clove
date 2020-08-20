#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"

#include "Clove/Graphics/Vulkan/VulkanHelpers.hpp"

namespace clv::gfx::vk {
    VKRenderPass::VKRenderPass(DevicePointer device, RenderPass::Descriptor descriptor)
        : device(std::move(device)) {
        //Attachments
        const size_t attachmentSize = std::size(descriptor.attachments);
        std::vector<VkAttachmentDescription> attachments(attachmentSize);
        for(size_t i = 0; i < attachmentSize; ++i) {
            VkAttachmentDescription attachment{};
            attachment.format         = convertImageFormat(descriptor.attachments[i].format);
            attachment.samples        = VK_SAMPLE_COUNT_1_BIT;
            attachment.loadOp         = convertLoadOp(descriptor.attachments[i].loadOperation);
            attachment.storeOp        = convertStoreOp(descriptor.attachments[i].storeOperation);
            attachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            attachment.initialLayout  = convertImageLayout(descriptor.attachments[i].initialLayout);
            attachment.finalLayout    = convertImageLayout(descriptor.attachments[i].finalLayout);

            attachments[i] = std::move(attachment);
        }

        //Subpasses
        const size_t subpassesSize = std::size(descriptor.subpasses);
        std::vector<VkSubpassDescription> subpasses(subpassesSize);
        std::vector<std::vector<VkAttachmentReference>> attachmentReferences(subpassesSize);//Define the references seperately so they aren't destroyed
        for(size_t i = 0; i < subpassesSize; ++i) {
            //Attachment References: Colour
            const size_t colourAttachmentSize = std::size(descriptor.subpasses[i].colourAttachments);
            attachmentReferences[i].resize(colourAttachmentSize);
            for(size_t j = 0; j < colourAttachmentSize; ++j) {
                VkAttachmentReference reference{};
                reference.attachment = descriptor.subpasses[i].colourAttachments[j].attachmentIndex;
                reference.layout     = convertImageLayout(descriptor.subpasses[i].colourAttachments[j].layout);

                attachmentReferences[i][j] = std::move(reference);
            }

            VkSubpassDescription subpass{};
            subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;//TODO: Only supporting graphics for now
            subpass.colorAttachmentCount = colourAttachmentSize;
            subpass.pColorAttachments    = std::data(attachmentReferences[i]);
            if(descriptor.subpasses[i].depthAttachment) {
                VkAttachmentReference reference{};
                reference.attachment = descriptor.subpasses[i].depthAttachment->attachmentIndex;
                reference.layout     = convertImageLayout(descriptor.subpasses[i].depthAttachment->layout);

                subpass.pDepthStencilAttachment = &reference;
            } else {
                subpass.pDepthStencilAttachment = nullptr;
            }

            subpasses[i] = std::move(subpass);
        }

        //Dependencies
        const size_t dependecySize = std::size(descriptor.dependencies);
        std::vector<VkSubpassDependency> dependecies(dependecySize);
        for(size_t i = 0; i < dependecySize; ++i) {
            VkSubpassDependency dependency{};
            dependency.srcSubpass    = descriptor.dependencies[i].sourceSubpass == SUBPASS_EXTERNAL ? VK_SUBPASS_EXTERNAL : descriptor.dependencies[i].sourceSubpass;
            dependency.dstSubpass    = descriptor.dependencies[i].destinationSubpass == SUBPASS_EXTERNAL ? VK_SUBPASS_EXTERNAL : descriptor.dependencies[i].destinationSubpass;
            dependency.srcStageMask  = convertPipelineStage(descriptor.dependencies[i].sourceStage);
            dependency.dstStageMask  = convertPipelineStage(descriptor.dependencies[i].destinationStage);
            dependency.srcAccessMask = convertAccessFlags(descriptor.dependencies[i].sourceAccess);
            dependency.dstAccessMask = convertAccessFlags(descriptor.dependencies[i].destinationAccess);

            dependecies[i] = std::move(dependency);
        }

        //Renderpass
        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = attachmentSize;
        renderPassInfo.pAttachments    = std::data(attachments);
        renderPassInfo.subpassCount    = subpassesSize;
        renderPassInfo.pSubpasses      = std::data(subpasses);
        renderPassInfo.dependencyCount = dependecySize;
        renderPassInfo.pDependencies   = std::data(dependecies);

        if(vkCreateRenderPass(this->device.get(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create render pass");
        }
    }

    VKRenderPass::VKRenderPass(VKRenderPass&& other) noexcept = default;

    VKRenderPass& VKRenderPass::operator=(VKRenderPass&& other) noexcept = default;

    VKRenderPass::~VKRenderPass() {
        vkDestroyRenderPass(device.get(), renderPass, nullptr);
    }

    VkRenderPass VKRenderPass::getRenderPass() const {
        return renderPass;
    }
}