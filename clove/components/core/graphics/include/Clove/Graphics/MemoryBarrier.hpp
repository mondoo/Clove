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
        AccessFlags sourceAccess;
        AccessFlags destinationAccess;
        QueueType sourceQueue{ QueueType::None };      /**< If set signifies which queue this is being transfered from. */
        QueueType destinationQueue{ QueueType::None }; /**< If set signifies which queue this is being transfered to. */
    };

    struct ImageMemoryBarrierInfo {
        AccessFlags sourceAccess;
        AccessFlags destinationAccess;
        GraphicsImage::Layout oldImageLayout;
        GraphicsImage::Layout newImageLayout;
        QueueType sourceQueue{ QueueType::None };      /**< If set signifies which queue this is being transfered from. */
        QueueType destinationQueue{ QueueType::None }; /**< If set signifies which queue this is being transfered to. */
    };
}