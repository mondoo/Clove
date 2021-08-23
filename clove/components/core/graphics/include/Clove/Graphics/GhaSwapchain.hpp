#pragma once

#include "Clove/Graphics/GhaImage.hpp"
#include "Clove/Graphics/Result.hpp"

#include <Clove/Maths/Vector.hpp>
#include <vector>

namespace clove {
    class GhaSemaphore;
    class GhaImageView;
}

namespace clove {
    /**
     * @brief A GhaSwapchain allows presentation of rendering results to a Window
     */
    class GhaSwapchain {
        //TYPES
    public:
        struct Descriptor {
            vec2ui extent;            /**< Size of the swapchain's backing images. */
            uint32_t imageCount{ 3 }; /**< How many images the swapchain will hold. */
        };

        //FUNCTIONS
    public:
        virtual ~GhaSwapchain() = default;

        /**
         * @brief Aquire the next available image that can be rendered to.
         * @param availableSemaphore A Sempahore that the GhaSwapchain will signal when the image is ready, can be nullptr.
         * @return Returns a pair that contains an index to the GhaImageView in the array retrieved from getImageViews
         * and a Result for if the operation was successful.
         */
        virtual std::pair<uint32_t, Result> aquireNextImage(GhaSemaphore const *availableSemaphore) = 0;

        virtual GhaImage::Format getImageFormat() const = 0;
        virtual vec2ui getSize() const                  = 0;

        /**
         * @brief Return the image views backing this swapchain. The lifetime
         * of the views are tied to this object.
         * @return 
         */
        virtual std::vector<GhaImageView *> getImageViews() const = 0;
    };
}
