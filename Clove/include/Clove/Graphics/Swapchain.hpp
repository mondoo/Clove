#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
    class Semaphore;
    class GraphicsImageView;
}

namespace clv::gfx {
    class Swapchain {
        //TYPES
    public:
        struct Descriptor {
            clv::mth::vec2ui extent;
        };

        //FUNCTIONS
    public:
        virtual ~Swapchain() = default;

        virtual ImageFormat getImageFormat() const = 0;
        virtual clv::mth::vec2ui getExtent() const = 0;

        virtual Result aquireNextImage(const Semaphore* semaphore, uint32_t& outImageIndex) = 0;

        virtual std::vector<std::shared_ptr<GraphicsImageView>> getImageViews() const = 0;
    };
}