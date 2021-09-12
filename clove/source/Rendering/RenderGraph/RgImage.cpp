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

    RgImage::RgImage(RgResourceIdType const id, GhaImage::Type const imageType, GhaImage::Format const format, vec2ui const dimensions, uint32_t const arrayCount)
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

    RgImage::RgImage(RgResourceIdType const id, GhaImage *ghaImage)
        : RgResource{ id }
        , ghaImage{ ghaImage }
        , ghaImageDescriptor{ ghaImage->getDescriptor() } {
        externalImage = true;
    }

    RgImage::RgImage(RgImage &&other) noexcept = default;

    RgImage &RgImage::operator=(RgImage &&other) noexcept = default;

    RgImage::~RgImage() = default;

    GhaImage *RgImage::getGhaImage(RgFrameCache &cache) {
        if(ghaImage == nullptr) {
            CLOVE_ASSERT_MSG(!externalImage, "RgImage is registered as an external image but does not have a valid GhaImageView.");
            ghaImage = cache.allocateImage(ghaImageDescriptor);
        }

        return ghaImage;
    }

    GhaImageView *RgImage::createGhaImageView(RgFrameCache &cache, uint32_t const arrayIndex, uint32_t const arrayCount) {
        //TODO: This will allocate a new view per usage - rather than reusing them when possible
        return cache.allocateImageView(getGhaImage(cache), GhaImageView::Descriptor{
                                                               .type       = getViewType(ghaImageDescriptor.type),
                                                               .layer      = arrayIndex,
                                                               .layerCount = 1,
                                                           });
    }

    void RgImage::addImageUsage(GhaImage::UsageMode const usage) {
        CLOVE_ASSERT_MSG(!externalImage, "Cannot change usage mode. RgImage is registered as an external image.");
        ghaImageDescriptor.usageFlags |= usage;
    }
}