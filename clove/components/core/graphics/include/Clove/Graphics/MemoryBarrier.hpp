#pragma once

#include "Clove/Graphics/GraphicsImage.hpp"
#include "Clove/Graphics/GraphicsResource.hpp"

namespace garlic::clove {
    enum class QueueType {
        None,
        Graphics,
        Transfer
    };

    struct BufferMemoryBarrierInfo {
        AccessFlags currentAccess{ AccessFlags::None }; /**< How the buffer is currently being used / accessed. */
        AccessFlags newAccess{ AccessFlags::None };     /**< How the buffer is going to be used / accessed. */
        QueueType sourceQueue{ QueueType::None };       /**< If set signifies which queue this is being transfered from. */
        QueueType destinationQueue{ QueueType::None };  /**< If set signifies which queue this is being transfered to. */
    };

    struct ImageMemoryBarrierInfo {
        AccessFlags currentAccess{ AccessFlags::None };                             /**< How the image is currently being used / accessed. */
        AccessFlags newAccess{ AccessFlags::None };                                 /**< How the image is going to be used / accessed. */
        GraphicsImage::Layout currentImageLayout{ GraphicsImage::Layout::General }; /**< The current layout of the image. */
        GraphicsImage::Layout newImageLayout{ GraphicsImage::Layout::General };     /**< The layout of the image to transition to after this barrier. */
        QueueType sourceQueue{ QueueType::None };                                   /**< If set signifies which queue this is being transfered from. */
        QueueType destinationQueue{ QueueType::None };                              /**< If set signifies which queue this is being transfered to. */
    };
}