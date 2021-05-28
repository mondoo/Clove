#include "Clove/Graphics/Metal/MetalImageView.hpp"

#include "Clove/Graphics/GhaImageView.hpp"

namespace garlic::clove {
	MetalImageView::MetalImageView(GhaImage::Format viewedFormat, vec2ui viewedDimensions, GhaImage *viewedImage, id<MTLTexture> texture)
		: viewedFormat{ viewedFormat }
        , viewedDimensions{ std::move(viewedDimensions) }
		, texture{ texture } {
	}
	
	MetalImageView::MetalImageView(MetalImageView &&other) noexcept = default;
	
	MetalImageView& MetalImageView::operator=(MetalImageView &&other) noexcept = default;
	
	MetalImageView::~MetalImageView() {
		[texture release];
	}

	GhaImage::Format MetalImageView::getImageFormat() const {
		return viewedFormat;
	}

    vec2ui const &MetalImageView::getImageDimensions() const {
		return viewedDimensions
	}
	
	id<MTLTexture> MetalImageView::getTexture() const {
		return texture;
	}
}
