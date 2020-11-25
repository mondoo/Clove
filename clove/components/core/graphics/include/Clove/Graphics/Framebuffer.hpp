#pragma once

#include <memory>
#include <vector>

namespace garlic::clove {
    class RenderPass;
    class GraphicsImageView;
}

namespace garlic::clove {
    /**
     * @brief Provides attachments (as GraphicsImageViews) to a given RenderPass.
     */
    class Framebuffer {
        //TYPES
    public:
        struct Descriptor {
            std::shared_ptr<RenderPass> renderPass;
            std::vector<std::shared_ptr<GraphicsImageView>> attachments; /**< The order of the attachments here has to match the those in the corresponding render pass */
            uint32_t width{ 0 };
            uint32_t height{ 0 };
        };

        //VARIABLES
    public:
        virtual ~Framebuffer() = default;
    };
}