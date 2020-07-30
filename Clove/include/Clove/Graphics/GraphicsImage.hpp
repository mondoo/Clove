#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

#include <Root/Definitions.hpp>

namespace clv::gfx {
    class GraphicsImageView;
}

namespace clv::gfx {
    /**
     * @brief Represents an arbitrary multidimensional (up to 3) array of sections of memory. Used in graphics operations.
     * @details Images can't be written to by the CPU directly. Data must be transferred into an image using a GraphicsBuffer
     * and a TransferQueue.
     */
    class GraphicsImage {
        //TYPES
    public:
        using UsageType = uint8_t;
        enum class UsageMode : UsageType {
            TransferDestination    = 1 << 0, /**< To be used as a destination in a transfer operation */
            Sampled                = 1 << 1, /**< To be used in an image view that's sampled in a shader */
            ColourAttachment       = 1 << 2, /**< To be used in an image view for a frame buffer */
            DepthStencilAttachment = 1 << 3, /**< To be used in an image view for a depth / stencil attachment */
        };

        enum class Type {
            _2D,
            _3D
        };

        struct Descriptor {
            Type type;
            UsageMode usageFlags;
            clv::mth::vec2ui dimensions;
            ImageFormat format;
            SharingMode sharingMode;
            MemoryType memoryType;
        };

        //FUNCTIONS
    public:
        virtual ~GraphicsImage() = default;

        virtual std::unique_ptr<GraphicsImageView> createView() const = 0;
    };

    GARLIC_ENUM_BIT_FLAG_OPERATORS(GraphicsImage::UsageMode, GraphicsImage::UsageType)
}