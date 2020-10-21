#pragma once

#include <cinttypes>

namespace clv::gfx {
    /**
     * @brief Represents a view on a subset of a GraphicsImage.
     * @details GraphicsImages are not directly accessed by shaders for reading / writing.
     * Instead, a view onto that image is used.
     * @see GraphicsImage::createView
     */
    class GraphicsImageView {
        //TYPES
    public:
        enum class Type {
            _2D,
            _3D,
            Cube
        };

        struct Descriptor {
            Type type;
            uint32_t layer{ 0 };      /**< If the image is an array or cube map, which element to start on (0 indexed). */
            uint32_t layerCount{ 0 }; /**< If the image is an array or cube map, how many elements to view. */
        };

        //FUNCTIONS
    public:
        virtual ~GraphicsImageView() = default;
    };
}