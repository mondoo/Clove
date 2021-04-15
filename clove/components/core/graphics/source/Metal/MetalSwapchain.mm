#include "Clove/Graphics/Metal/MetalSwapchain.hpp"

#include <QuartzCore/CAMetalLayer.h>

namespace garlic::clove {
	MetalSwapchain::MetalSwapchain(MTKView *view)
		: view{ view } {
	}
	
	MetalSwapchain::MetalSwapchain(MetalSwapchain &&other) noexcept = default;
	
	MetalSwapchain& MetalSwapchain::operator=(MetalSwapchain &&other) noexcept = default;
	
	MetalSwapchain::~MetalSwapchain() {
		[view release];
	}
	
	std::pair<uint32_t, Result> MetalSwapchain::aquireNextImage(GhaSemaphore const *availableSemaphore) {
		//TODO: This will cause the layer to wait until a new drawable is free. Is it possible to set up dummy views that we can set later?
		[[((CAMetalLayer*) [view layer]) nextDrawable] texture];
		return {};
	}

	GhaImage::Format MetalSwapchain::getImageFormat() const {
		//TODO
		return {};
	}
	
	vec2ui MetalSwapchain::getSize() const {
		//TODO
		return {};
	}

	std::vector<std::shared_ptr<GhaImageView>> MetalSwapchain::getImageViews() const {
		//TODO
		return {};
	}
}
