#pragma once

#include "Clove/Graphics/GraphicsImage.hpp"
#include "Clove/Graphics/Result.hpp"

namespace clv::gfx {
    class Semaphore;
    class GraphicsImageView;
}

namespace clv::gfx {
    /**
     * @brief A Swapchain allows presentation of rendering results to a Window
     */
    class Swapchain {
        //TYPES
    public:
        struct Descriptor {
            clv::mth::vec2ui extent;
        };

        //FUNCTIONS
    public:
        virtual ~Swapchain() = default;

        virtual GraphicsImage::Format getImageFormat() const = 0;
        virtual clv::mth::vec2ui getExtent() const           = 0;

        virtual Result aquireNextImage(Semaphore const *semaphore, uint32_t &outImageIndex) = 0;

        virtual std::vector<std::shared_ptr<GraphicsImageView>> getImageViews() const = 0;
    };
}