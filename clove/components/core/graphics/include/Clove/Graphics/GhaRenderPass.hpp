#pragma once

#include "Clove/Graphics/GhaImage.hpp"
#include "Clove/Graphics/Resource.hpp"
#include "Clove/Graphics/GhaPipelineObject.hpp"

#include <optional>
#include <vector>

namespace garlic::clove {
    /** Used to represent the implicit subpass before or after a render pass */
    inline constexpr uint32_t SUBPASS_EXTERNAL = ~0U;

    enum class LoadOperation {
        Load,
        Clear,
        DontCare,
    };

    enum class StoreOperation {
        Store,
        DontCare,
    };

    /**
     * @brief Describes what happens to an attachment when it is loaded and stored.
     */
    struct AttachmentDescriptor {
        GhaImage::Format format{ GhaImage::Format::Unkown };
        LoadOperation loadOperation{ LoadOperation::DontCare };
        StoreOperation storeOperation{ StoreOperation::DontCare };
        GhaImage::Layout initialLayout{ GhaImage::Layout::Undefined }; /**< What layout the GhaRenderPass will expect the image to be in. */
        GhaImage::Layout finalLayout{ GhaImage::Layout::Undefined };   /**< What layout the image will be transitioned to at the end of the GhaRenderPass.*/
    };

    /**
     * @brief Allows an attachment to be referenced between multiple subpasses.
     */
    struct AttachmentReference {
        uint32_t attachmentIndex{ 0 };                          /**< The index of the attachment in RenderPassDescriptor::attachments this refernces uses. */
        GhaImage::Layout layout{ GhaImage::Layout::Undefined }; /**< What layout the attachment will transition to when used during the subpass. */
    };

    /**
     * @brief Describes which attachments a subpass uses.
     */
    struct SubpassDescriptor {
        std::vector<AttachmentReference> colourAttachments;
        std::optional<AttachmentReference> depthAttachment;
    };

    /**
     * @brief Describes a dependency between two subpasses, allowing a subpass to be executed after another.
     */
    struct SubpassDependency {
        uint32_t sourceSubpass;      /**< Index of the dependency subpass (inside RenderPassDescriptor::subpasses or SUBPASS_EXTERNAL) */
        uint32_t destinationSubpass; /**< Index of the dependent subpass (inside RenderPassDescriptor::subpasses or SUBPASS_EXTERNAL). Must be higher than sourceSubpass. */

        GhaPipelineObject::Stage sourceStage;      /**< The pipeline stage that gets executed before the dependency in sourceSubpass. */
        GhaPipelineObject::Stage destinationStage; /**< The pipeline stage executed after the barrier in destinationSubpass that waits for the results of the sourceStage. */

        AccessFlags currentAccess; /**< How the attachment(s) in the sourceSubpass are currently being accessed. */
        AccessFlags newAccess;     /**< How the attachment(s) in the destinationSubpass will be accessed. */
    };

    /**
     * @brief A GhaRenderPass is a collection of attachments, subpasses and their dependecies.
     * @details A GhaRenderPass only describes the attachments used in the pipeline. The actual
     * array of attachments are in a GhaFramebuffer.
     */
    class GhaRenderPass {
        //TYPES
    public:
        struct Descriptor {
            std::vector<AttachmentDescriptor> attachments;
            std::vector<SubpassDescriptor> subpasses;
            std::vector<SubpassDependency> dependencies;
        };

        //FUNCTIONS
    public:
        virtual ~GhaRenderPass() = default;
    };
}