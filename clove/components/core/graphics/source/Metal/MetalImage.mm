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
	
	MTLPixelFormat MetalImage::convertFormat(Format format) {
		switch(format) {
			case GhaImage::Format::Unkown:
				return MTLPixelFormatInvalid;
			case GhaImage::Format::R8_UNORM:
				return MTLPixelFormatR8Unorm;
			case GhaImage::Format::R8G8B8A8_SRGB:
				return MTLPixelFormatRGBA8Unorm_sRGB;
			case GhaImage::Format::B8G8R8A8_SRGB:
				return MTLPixelFormatBGRA8Unorm_sRGB;
			case GhaImage::Format::B8G8R8A8_UNORM:
				return MTLPixelFormatBGRA8Unorm;
			case GhaImage::Format::D32_SFLOAT:
				return MTLPixelFormatDepth32Float;
			default:
				CLOVE_ASSERT(false, "{0}: Unkown format", CLOVE_FUNCTION_NAME);
				return MTLPixelFormatInvalid;
		}
	}
}
