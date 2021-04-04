#pragma once

#include <Clove/Graphics/GhaImage.hpp>
#include <Clove/Graphics/GhaImageView.hpp>
#include <Clove/Maths/Vector.hpp>
#include <memory>

namespace garlic::clove {
    class GhaFactory;
}

namespace garlic::clove {
    /**
     * @brief Tracks the usage and dependencies of an image in the RenderGraph.
     * @details Can be used to later create a GhaImage when executing the graph.
     */
    class RgImage {
        friend class RenderGraph;

        //VARIABLES
    private:
        GhaImage::Descriptor ghaDescriptor{}; /**< Gets updated while the image is used within the graph. */

        //TODO: Keep some sort of internal ID (and maybe pointer to GhaImage) so when copying around it's easy to tell which RgImages are unique etc.

        //FUNCTIONS
    public:
        RgImage();
        RgImage(GhaImage::Type imagetype, vec2ui dimensions);
        RgImage(std::shared_ptr<GhaImageView> ghaImageView);

        RgImage(RgImage const &other);
        RgImage(RgImage &&other) noexcept;

        RgImage &operator=(RgImage const &other);
        RgImage &operator=(RgImage &&other) noexcept;

        ~RgImage();

    private:
        /**
         * @brief Creates a GhaImage with it's usage/layout set based on how it's used in the graph. 
         * @param factory 
         * @return 
         */
        std::unique_ptr<GhaImage> createGhaImage(GhaFactory &factory);
    };
}