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

    RgImage::RgImage(GhaImage::Type const imageType, GhaImage::Format const format, vec2ui const dimensions, uint32_t const arrayCount) {
        ghaImageDescriptor = GhaImage::Descriptor{
            .type        = imageType,
            .usageFlags  = static_cast<GhaImage::UsageMode>(0),//Will be built when executing the graph
            .dimensions  = dimensions,
            .arrayCount  = arrayCount,
            .format      = format,
            .sharingMode = SharingMode::Exclusive,//Images are always exclusive.
        };
    }

    RgImage::RgImage(GhaImage *ghaImage)
        : ghaImage{ ghaImage }
        , ghaImageDescriptor{ ghaImage->getDescriptor() } {
        externalImage = true;
    }

    RgImage::RgImage(RgImage &&other) noexcept = default;

    RgImage &RgImage::operator=(RgImage &&other) noexcept = default;

    RgImage::~RgImage() = default;

    RgImageView::RgImageView(RgImage *image, uint32_t const arrayIndex, uint32_t const arrayCount)
        : image{ image }
        , arrayIndex{ arrayIndex }
        , arrayCount{ arrayCount } {
    }

    RgImageView::RgImageView(RgImageView &&other) noexcept = default;

    RgImageView &RgImageView::operator=(RgImageView &&other) noexcept = default;

    RgImageView::~RgImageView() = default;

    GhaImage *RgImageView::getGhaImage(RgFrameCache &cache) {
        if(image->ghaImage == nullptr) {
            CLOVE_ASSERT_MSG(!image->externalImage, "RgImage is registered as an external image but does not have a valid GhaImageView.");
            image->ghaImage = cache.allocateImage(image->ghaImageDescriptor);
        }

        return image->ghaImage;
    }

    GhaImageView *RgImageView::getGhaImageView(RgFrameCache &cache) {
        if(ghaImageView == nullptr) {
            ghaImageView = cache.allocateImageView(getGhaImage(cache), GhaImageView::Descriptor{
                                                                           .type       = getViewType(image->ghaImageDescriptor.type),
                                                                           .layer      = arrayIndex,
                                                                           .layerCount = arrayCount,
                                                                       });
        }

        return ghaImageView;
    }

    void RgImageView::addImageUsage(GhaImage::UsageMode const usage) {
        CLOVE_ASSERT_MSG(!image->externalImage, "Cannot change usage mode. RgImage is registered as an external image.");
        image->ghaImageDescriptor.usageFlags |= usage;
    }
}