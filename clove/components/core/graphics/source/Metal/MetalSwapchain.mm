#include "Clove/Graphics/Metal/MetalSwapchain.hpp"

#include "Clove/Graphics/Metal/MetalImage.hpp"
#include "Clove/Graphics/Metal/MetalView.hpp"

#include <QuartzCore/CAMetalLayer.h>

namespace garlic::clove {
	MetalSwapchain::MetalSwapchain(MetalView *view)
		: view{ view } {
	}
	
	MetalSwapchain::MetalSwapchain(MetalSwapchain &&other) noexcept = default;
	
	MetalSwapchain& MetalSwapchain::operator=(MetalSwapchain &&other) noexcept = default;
	
	MetalSwapchain::~MetalSwapchain() {
		[view release];
	}
	
	std::pair<uint32_t, Result> MetalSwapchain::aquireNextImage(GhaSemaphore const *availableSemaphore) {
		//TODO
		return {};
	}

	GhaImage::Format MetalSwapchain::getImageFormat() const {
		return MetalImage::convertFormat([view.metalLayer pixelFormat]);
	}
	
	vec2ui MetalSwapchain::getSize() const {
		CGSize const size{ [view.metalLayer drawableSize] };
		return { size.width, size.height };
	}

	std::vector<std::shared_ptr<GhaImageView>> MetalSwapchain::getImageViews() const {
		//TODO
		return {};
	}
}
