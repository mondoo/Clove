#pragma once

//TODO: Remove
#include <vulkan/vulkan.h>
#include "Clove/Graphics/Vulkan/VKSwapchain.hpp"

#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

namespace clv::gfx {
    struct AttachmentDescriptor {
        ImageFormat format{ ImageFormat::Unkown };
        LoadOperation loadOperation{ LoadOperation::DontCare };
        StoreOperation storeOperation{ StoreOperation::DontCare };
        ImageLayout initialLayout{ ImageLayout::Undefined };
        ImageLayout finalLayout{ ImageLayout::Undefined };
        
    };

    struct AttachmentReference {
        uint32_t attachmentIndex{ 0 };//the index of the attachment in RenderPassDescriptor::attachments this refernces uses
        ImageLayout layout{ ImageLayout::Undefined };
    };

    struct SubpassDescriptor {
        std::vector<AttachmentReference> colourAttachments;
        std::optional<AttachmentReference> depthAttachment;
    };

    struct SubpassDependecy {
        uint32_t sourceSubpass;     //The dependency subpass in RenderPassDescriptor::subpasses
        uint32_t destinationSubpass;//The dependent subpass in RenderPassDescriptor::subpasses
        //Which stage the operations to wait on occurs
        PipelineStage sourceStage;
        PipelineStage destinationStage;
        //The operations to wait on
        AccessType sourceAccess;
        AccessType destinationAccess;
    };

    struct RenderPassDescriptor {
        std::vector<AttachmentDescriptor> attachments;
        std::vector<SubpassDescriptor> subpasses;
        std::vector<SubpassDependecy> dependencies;
    };
}

namespace clv::gfx::vk {
	class VKRenderPass {
		//VARIABLES
	private:
		VkDevice device = VK_NULL_HANDLE;

		VkRenderPass renderPass = VK_NULL_HANDLE;

		//FUNCTIONS
	public:
		//TODO: ctors
		VKRenderPass(VkDevice device, RenderPassDescriptor descriptor);
		~VKRenderPass();

		//VK specific
		VkRenderPass getRenderPass() const;
	};
}