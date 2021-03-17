#pragma once

#include "Clove/Graphics/GhaGraphicsQueue.hpp"

#include <MetalKit/MetalKit.h>

namespace garlic::clove {
	class MetalGraphicsQueue : public GhaGraphicsQueue {
		//VARIABLES
	private:
		id<MTLCommandQueue> commandQueue;

		//FUNCTIONS
	public:
		//TODO: Ctors
		//~MetalGraphicsQueue();

		std::unique_ptr<GhaGraphicsCommandBuffer> allocateCommandBuffer() override;
		void freeCommandBuffer(GhaGraphicsCommandBuffer &buffer) override;

		void submit(std::vector<GraphicsSubmitInfo> const &submissions, GhaFence const *signalFence) override;
	};
}

