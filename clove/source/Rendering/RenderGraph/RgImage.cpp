#include "Clove/Rendering/RenderGraph/RgImage.hpp"

#include <Clove/Graphics/GhaFactory.hpp>

namespace garlic::clove {
    RgImage::RgImage() = default;

    RgImage::RgImage(GhaImage::Type imagetype, vec2ui dimensions) {
        ghaDescriptor.dimensions = std::move(dimensions);
        ghaDescriptor.type       = imagetype;
    }

    RgImage::RgImage(RgImage const &other) {
        //TODO
    }

    RgImage::RgImage(RgImage &&other) noexcept {
        //TODO: Does usage needed to be tracked here?
    }

    RgImage &RgImage::operator=(RgImage const &other) {
        //TODO
        return *this;
    }

    RgImage &RgImage::operator=(RgImage &&other) noexcept {
        //TODO: Does usage needed to be tracked here?
        return *this;
    }

    RgImage::~RgImage() = default;

    std::unique_ptr<GhaImage> RgImage::createGhaImage(GhaFactory &factory) {
        return *factory.createImage(ghaDescriptor);
    }
}