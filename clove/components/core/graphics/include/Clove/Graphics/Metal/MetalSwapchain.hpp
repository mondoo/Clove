#pragma once

#include "Clove/Graphics/GhaSwapchain.hpp"

#include <MetalKit/MetalKit.h>

namespace garlic::clove {
	class MetalSwapchain : public GhaSwapchain {
		//VARIABLES
	private:
		MTKView *view{ nullptr };
		
		//FUNCTIONS
	public:
		MetalSwapchain() = delete;
		MetalSwapchain(MTKView *view);
		
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
