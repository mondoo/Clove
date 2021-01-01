#pragma once

#include "Clove/Graphics/GraphicsImage.hpp"
#include "Clove/Graphics/GraphicsResource.hpp"
#include "Clove/Graphics/PipelineObject.hpp"

#include <optional>
#include <vector>

namespace garlic::clove {
    /** Used to represent the implicit subpass before or after a render pass */
    inline constexpr uint32_t SUBPASS_EXTERNAL = ~0U;

    enum class LoadOperation {
        DontCare,
        Clear,
    };

    enum class StoreOperation {
        DontCare,
        Store,
    };

    /**
     * @brief Describes what happens to an attachment when it is loaded and stored.
     */
    struct AttachmentDescriptor {
        GraphicsImage::Format format{ GraphicsImage::Format::Unkown };
        LoadOperation loadOperation{ LoadOperation::DontCare };
        StoreOperation storeOperation{ StoreOperation::DontCare };
        GraphicsImage::Layout initialLayout{ GraphicsImage::Layout::Undefined }; /**< What layout the RenderPass will expect the image to be in. */
        GraphicsImage::Layout finalLayout{ GraphicsImage::Layout::Undefined };   /**< What layout the image will be transitioned to at the end of the RenderPass.*/
    };

    /**
     * @brief Allows an attachment to be referenced between multiple subpasses.
     */
    struct AttachmentReference {
        uint32_t attachmentIndex{ 0 };                                    /**< The index of the attachment in RenderPassDescriptor::attachments this refernces uses. */
        GraphicsImage::Layout layout{ GraphicsImage::Layout::Undefined }; /**< What layout the attachment will transition to when used during the subpass. */
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

        PipelineObject::Stage sourceStage;      /**< The pipeline stage that gets executed before the dependency in sourceSubpass. */
        PipelineObject::Stage destinationStage; /**< The pipeline stage executed after the barrier in destinationSubpass that waits for the results of the sourceStage. */

        AccessFlags currentAccess; /**< How the attachment(s) in the sourceSubpass are currently being accessed. */
        AccessFlags newAccess;     /**< How the attachment(s) in the destinationSubpass will be accessed. */
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