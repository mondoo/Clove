#pragma once

#include "Clove/Graphics/GhaSwapchain.hpp"

#include <MetalKit/MetalKit.h>

@class MetalView;

namespace garlic::clove {
	class MetalSwapchain : public GhaSwapchain {
		//VARIABLES
	private:
		MetalView *view{ nullptr };
		
		//FUNCTIONS
	public:
		MetalSwapchain() = delete;
		MetalSwapchain(MetalView *view);
		
		MetalSwapchain(MetalSwapchain const &other) = delete;
		MetalSwapchain(MetalSwapchain &&other) noexcept;
		
		MetalSwapchain& operator=(MetalSwapchain const &other) = delete;
		MetalSwapchain& operator=(MetalSwapchain &&other) noexcept;
		
		~MetalSwapchain();
		
		std::pair<uint32_t, Result> aquireNextImage(GhaSemaphore const *availableSemaphore) override;

		GhaImage::Format getImageFormat() const override;
		vec2ui getSize() const override;

		std::vector<std::shared_ptr<GhaImageView>> getImageViews() const override;
	};
}
