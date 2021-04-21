#include "Clove/Graphics/Metal/MetalPresentQueue.hpp"

#include "Clove/Graphics/Metal/MetalView.hpp"

namespace garlic::clove {
	MetalPresentQueue::MetalPresentQueue(id<MTLCommandQueue> commandQueue, MetalView *view)
		: commandQueue{ commandQueue }
		, view{ view } {
	}
	
	MetalPresentQueue::MetalPresentQueue(MetalPresentQueue &&other) noexcept = default;
	
	MetalPresentQueue &MetalPresentQueue::operator=(MetalPresentQueue &&other) noexcept = default;
	
	MetalPresentQueue::~MetalPresentQueue() {
		[commandQueue release];
		[view release];
	}
	
	Result MetalPresentQueue::present(PresentInfo const &presentInfo) {
		//Get image from swap chain. Copy everything to that drawable
	}
}
