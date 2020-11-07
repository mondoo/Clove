#pragma once

#include "Clove/Graphics/GraphicsImageView.hpp"
#include "Clove/Graphics/GraphicsResource.hpp"

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
        using UsageModeType = uint8_t;
        enum class UsageMode : UsageModeType {
            TransferDestination    = 1 << 0, /**< To be used as a destination in a transfer operation. Such as writing data from a system memory backed buffer to a video memory backed buffer */
            Sampled                = 1 << 1, /**< To be used in a GraphicsImageView that's sampled in a shader */
            ColourAttachment       = 1 << 2, /**< To be used in a GraphicsImageView for a frame buffer */
            DepthStencilAttachment = 1 << 3, /**< To be used in a GraphicsImageView for a depth / stencil attachment */
        };

        enum class Type {
            _2D,
            _3D,
            Cube
        };

        enum class Format {
            Unkown,

            R8G8B8A8_SRGB,
            B8G8R8A8_SRGB,
            B8G8R8A8_UNORM,

            D32_SFLOAT
        };

        enum class Layout {
            Undefined,
            General,
            Present,
            TransferDestinationOptimal,
            ShaderReadOnlyOptimal,
            ColourAttachmentOptimal,
            DepthStencilAttachmentOptimal,
            DepthStencilReadOnlyOptimal
        };

        struct Descriptor {
            Type type;
            UsageMode usageFlags;
            clv::mth::vec2ui dimensions;
            Format format;
            SharingMode sharingMode;
        };

        //FUNCTIONS
    public:
        virtual ~GraphicsImage() = default;

        virtual std::unique_ptr<GraphicsImageView> createView(GraphicsImageView::Descriptor viewDescriptor) const = 0;
    };

    GARLIC_ENUM_BIT_FLAG_OPERATORS(GraphicsImage::UsageMode, GraphicsImage::UsageModeType)
}