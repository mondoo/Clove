#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"

#include "Clove/Graphics/Vulkan/VKGraphicsResource.hpp"
#include "Clove/Graphics/Vulkan/VKImage.hpp"
#include "Clove/Graphics/Vulkan/VKPipelineObject.hpp"

#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    static VkAttachmentLoadOp convertLoadOp(LoadOperation garlicOperation) {
        switch(garlicOperation) {
            case LoadOperation::DontCare:
                return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            case LoadOperation::Clear:
                return VK_ATTACHMENT_LOAD_OP_CLEAR;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown operation", GARLIC_FUNCTION_NAME);
                return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        }
    }

    static VkAttachmentStoreOp convertStoreOp(StoreOperation garlicOperation) {
        switch(garlicOperation) {
            case StoreOperation::DontCare:
                return VK_ATTACHMENT_STORE_OP_DONT_CARE;
            case StoreOperation::Store:
                return VK_ATTACHMENT_STORE_OP_STORE;
            default:
                GARLIC_ASSERT(false, "{0}: Unkown operation", GARLIC_FUNCTION_NAME);
                return VK_ATTACHMENT_STORE_OP_DONT_CARE;
        }
    }

    VKRenderPass::VKRenderPass(DevicePointer device, RenderPass::Descriptor descriptor)
        : device(std::move(device)) {
        //Attachments
        const size_t attachmentSize = std::size(descriptor.attachments);
        std::vector<VkAttachmentDescription> attachments(attachmentSize);
        for(size_t i = 0; i < attachmentSize; ++i) {
            VkAttachmentDescription attachment{
                .format         = VKImage::convertFormat(descriptor.attachments[i].format),
                .samples        = VK_SAMPLE_COUNT_1_BIT,
                .loadOp         = convertLoadOp(descriptor.attachments[i].loadOperation),
                .storeOp        = convertStoreOp(descriptor.attachments[i].storeOperation),
                .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout  = VKImage::convertLayout(descriptor.attachments[i].initialLayout),
                .finalLayout    = VKImage::convertLayout(descriptor.attachments[i].finalLayout),
            };
            attachments[i] = std::move(attachment);
        }

        //Subpasses
        const size_t subpassesSize = std::size(descriptor.subpasses);
        std::vector<VkSubpassDescription> subpasses(subpassesSize);

        //Define the references seperately so they aren't destroyed
        std::vector<std::vector<VkAttachmentReference>> attachmentReferences(subpassesSize);
        std::vector<std::optional<VkAttachmentReference>> depthStencilAttachment(subpassesSize);

        for(size_t i = 0; i < subpassesSize; ++i) {
            //Attachment References: Colour
            const size_t colourAttachmentSize = std::size(descriptor.subpasses[i].colourAttachments);
            attachmentReferences[i].resize(colourAttachmentSize);
            for(size_t j = 0; j < colourAttachmentSize; ++j) {
                VkAttachmentReference reference{
                    .attachment = descriptor.subpasses[i].colourAttachments[j].attachmentIndex,
                    .layout     = VKImage::convertLayout(descriptor.subpasses[i].colourAttachments[j].layout),
                };

                attachmentReferences[i][j] = std::move(reference);
            }

            //Attachment References: Depth
            if(descriptor.subpasses[i].depthAttachment) {
                VkAttachmentReference reference{
                    .attachment = descriptor.subpasses[i].depthAttachment->attachmentIndex,
                    .layout     = VKImage::convertLayout(descriptor.subpasses[i].depthAttachment->layout),
                };

                depthStencilAttachment[i] = std::move(reference);
            }

            VkSubpassDescription subpass{
                .flags                   = 0,
                .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,//TODO: Only supporting graphics for now
                .inputAttachmentCount    = 0,
                .pInputAttachments       = nullptr,
                .colorAttachmentCount    = static_cast<uint32_t>(colourAttachmentSize),
                .pColorAttachments       = std::data(attachmentReferences[i]),
                .pResolveAttachments     = nullptr,
                .pDepthStencilAttachment = depthStencilAttachment[i].has_value() ? &depthStencilAttachment[i].value() : nullptr,
                .preserveAttachmentCount = 0,
                .pPreserveAttachments    = nullptr,
            };

            subpasses[i] = std::move(subpass);
        }

        //Dependencies
        const size_t dependecySize = std::size(descriptor.dependencies);
        std::vector<VkSubpassDependency> dependecies(dependecySize);
        for(size_t i = 0; i < dependecySize; ++i) {
            VkSubpassDependency dependency{
                .srcSubpass    = descriptor.dependencies[i].sourceSubpass == SUBPASS_EXTERNAL ? VK_SUBPASS_EXTERNAL : descriptor.dependencies[i].sourceSubpass,
                .dstSubpass    = descriptor.dependencies[i].destinationSubpass == SUBPASS_EXTERNAL ? VK_SUBPASS_EXTERNAL : descriptor.dependencies[i].destinationSubpass,
                .srcStageMask  = VKPipelineObject::convertStage(descriptor.dependencies[i].sourceStage),
                .dstStageMask  = VKPipelineObject::convertStage(descriptor.dependencies[i].destinationStage),
                .srcAccessMask = convertAccessFlags(descriptor.dependencies[i].sourceAccess),
                .dstAccessMask = convertAccessFlags(descriptor.dependencies[i].destinationAccess),
            };

            dependecies[i] = std::move(dependency);
        }

        //Renderpass
        VkRenderPassCreateInfo renderPassInfo{
            .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .attachmentCount = static_cast<uint32_t>(attachmentSize),
            .pAttachments    = std::data(attachments),
            .subpassCount    = static_cast<uint32_t>(subpassesSize),
            .pSubpasses      = std::data(subpasses),
            .dependencyCount = static_cast<uint32_t>(dependecySize),
            .pDependencies   = std::data(dependecies),
        };

        if(vkCreateRenderPass(this->device.get(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
            GARLIC_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to create render pass");
        }
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