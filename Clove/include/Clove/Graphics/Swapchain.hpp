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

        /**
         * @brief Aquire the next available image that can be rendered to.
         * @param availableSemaphore A Sempahore that the Swapchain will signal when the image is ready, can be nullptr.
         * @return Returns a pair that contains an index to the GraphicsImageView in the array retrieved from getImageViews
         * and a Result for if the operation was successful.
         */
        virtual std::pair<uint32_t, Result> aquireNextImage(Semaphore const *availableSemaphore) = 0;

        virtual GraphicsImage::Format getImageFormat() const = 0;
        virtual clv::mth::vec2ui getSize() const             = 0;

        virtual std::vector<std::shared_ptr<GraphicsImageView>> getImageViews() const = 0;
    };
}