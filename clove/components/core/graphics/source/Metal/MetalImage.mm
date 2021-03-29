#include "Clove/Graphics/Metal/MetalImage.hpp"

namespace garlic::clove {
	MetalImage::MetalImage(id<MTLTexture> texture, Descriptor descriptor)
		: texture{ texture }
		, descriptor{ std::move(descriptor) } {
	}
	
	MetalImage::MetalImage(MetalImage &&other) noexcept = default;
	
	MetalImage& MetalImage::operator=(MetalImage &&other) noexcept = default;
	
	MetalImage::~MetalImage() = default;
	
	GhaImage::Descriptor const &MetalImage::getDescriptor() const {
		return descriptor;
	}

	std::unique_ptr<GhaImageView> MetalImage::createView(GhaImageView::Descriptor viewDescriptor) const {
		//TODO:
		return nullptr;
	}
	
	id<MTLTexture> MetalImage::getTexture() const {
		return texture;
	}
}
