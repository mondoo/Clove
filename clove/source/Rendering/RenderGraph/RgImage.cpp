#include "Clove/Rendering/RenderGraph/RgImage.hpp"

#include "Clove/Rendering/RenderGraph/RgFrameCache.hpp"

#include <Clove/Log/Log.hpp>

namespace clove {
    namespace {
        GhaImageView::Type getViewType(GhaImage::Type imageType) {
            switch(imageType) {
                case GhaImage::Type::_2D:
                    return GhaImageView::Type::_2D;
                case GhaImage::Type::_3D:
                    return GhaImageView::Type::_3D;
                case GhaImage::Type::Cube:
                    return GhaImageView::Type::Cube;
            }
        }
    }

    RgImage::RgImage(RgResourceIdType id, GhaImage::Type imageType, GhaImage::Format format, vec2ui dimensions, uint32_t arrayCount)
        : RgResource{ id } {
        ghaImageDescriptor = GhaImage::Descriptor{
            .type        = imageType,
            .usageFlags  = static_cast<GhaImage::UsageMode>(0),//Will be built when executing the graph
            .dimensions  = dimensions,
            .arrayCount  = arrayCount,
            .format      = format,
            .sharingMode = SharingMode::Exclusive,//Images are always exclusive.
        };
    }

    RgImage::RgImage(RgResourceIdType id, GhaImage *ghaImage)
        : RgResource{ id }
        , ghaImage{ ghaImage } {
        externalImage = true;
    }

    RgImage::RgImage(RgImage &&other) noexcept = default;

    RgImage &RgImage::operator=(RgImage &&other) noexcept = default;

    RgImage::~RgImage() = default;

    GhaImageView *RgImage::getGhaImageView(RgFrameCache &cache, uint32_t const arrayIndex, uint32_t const arrayCount) {
        if(ghaImage == nullptr) {
            CLOVE_ASSERT(!externalImage, "RgImage is registered as an external image but does not have a valid GhaImageView.");
            ghaImage = cache.allocateImage(ghaImageDescriptor);
        }

        //TODO: This will allocate a new view per usage - rather than reusing them when possible
        ghaImageView = cache.allocateImageView(ghaImage, GhaImageView::Descriptor{
                                                             .type       = getViewType(ghaImageDescriptor.type),
                                                             .layer      = arrayIndex,
                                                             .layerCount = arrayCount,
                                                         });

        return ghaImageView;
    }

    void RgImage::addImageUsage(GhaImage::UsageMode usage) {
        CLOVE_ASSERT(!externalImage, "Cannot change usage mode. RgImage is registered as an external image.");
        ghaImageDescriptor.usageFlags |= usage;
    }
}