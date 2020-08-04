#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx {
    /** Used to represent the implicit subpass before or after a render pass */
    inline constexpr uint32_t SUBPASS_EXTERNAL = ~0U;

    /**
     * @brief Describes what happens to an attachment when it is loaded and stored.
     */
    struct AttachmentDescriptor {
        ImageFormat format{ ImageFormat::Unkown };
        LoadOperation loadOperation{ LoadOperation::DontCare };
        StoreOperation storeOperation{ StoreOperation::DontCare };
        ImageLayout initialLayout{ ImageLayout::Undefined };
        ImageLayout finalLayout{ ImageLayout::Undefined };
    };

    /**
     * @brief Allows an attachment to be referenced between multiple subpasses.
     */
    struct AttachmentReference {
        uint32_t attachmentIndex{ 0 }; /**< the index of the attachment in RenderPassDescriptor::attachments this refernces uses */
        ImageLayout layout{ ImageLayout::Undefined };
    };

    /**
     * @brief Describes which attachments a subpass uses.
     */
    struct SubpassDescriptor {
        std::vector<AttachmentReference> colourAttachments;
        std::optional<AttachmentReference> depthAttachment;
    };

    /**
     * @brief Describes a dependency between two subpasses for at what point can an attachment's layout be transitioned.
     */
    struct SubpassDependency {
        uint32_t sourceSubpass;      /**< Index of the dependency subpass (inside RenderPassDescriptor::subpasses or SUBPASS_EXTERNAL) */
        uint32_t destinationSubpass; /**< Index of the dependent subpass (inside RenderPassDescriptor::subpasses or SUBPASS_EXTERNAL) */

        PipelineStage sourceStage;      /**< Which PipelineStage on the sourceSubpass we wait on */
        PipelineStage destinationStage; /**< Which PipelineStage on the destinationSubpass that waits on this dependency */

        AccessType sourceAccess;      /**< Which operation on the sourceSubpass we wait on */
        AccessType destinationAccess; /**< Which operation on the destinationSubpass that should wait on this dependency */
    };

    /**
     * @brief A RenderPass is a collection of attachments, subpasses and their dependecies.
     * @details A RenderPass only describes the attachments used in the pipeline. The actual
     * array of attachments are in a Framebuffer.
     */
    class RenderPass {
        //TYPES
    public:
        struct Descriptor {
            std::vector<AttachmentDescriptor> attachments;
            std::vector<SubpassDescriptor> subpasses;
            std::vector<SubpassDependency> dependencies;
        };

        //FUNCTIONS
    public:
        virtual ~RenderPass() = default;
    };
}