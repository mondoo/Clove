#pragma once

namespace clv::gfx {
    /**
     * @brief Represents a view on a subset of a GraphicsImage.
     * @details GraphicsImages are not directly accessed by shaders for reading / writing.
     * Instead, a view onto that image is used.
     * @see GraphicsImage::createView
     */
    class GraphicsImageView {
        //FUNCTIONS
    public:
        virtual ~GraphicsImageView() = default;
    };
}