#include "Clove/Graphics/Metal/MetalGraphicsQueue.hpp"

#include "Clove/Graphics/Metal/MetalGraphicsCommandBuffer.hpp"
#include "Clove/Graphics/Metal/MetalView.hpp"
#include "Clove/Graphics/Metal/MetalFence.hpp"

#include <Clove/Cast.hpp>

namespace garlic::clove {
	MetalGraphicsQueue::MetalGraphicsQueue(id<MTLCommandQueue> commandQueue)
		: commandQueue{ [commandQueue retain] } {
	}
	
	MetalGraphicsQueue::MetalGraphicsQueue(MetalGraphicsQueue &&other) noexcept = default;
	
	MetalGraphicsQueue& MetalGraphicsQueue::operator=(MetalGraphicsQueue &&other) noexcept = default;
	
	MetalGraphicsQueue::~MetalGraphicsQueue() = default;
	
	std::unique_ptr<GhaGraphicsCommandBuffer> MetalGraphicsQueue::allocateCommandBuffer() {
		return std::make_unique<MetalGraphicsCommandBuffer>();
	}
	
	void MetalGraphicsQueue::freeCommandBuffer(GhaGraphicsCommandBuffer &buffer) {
		//no op
	}

	void MetalGraphicsQueue::submit(std::vector<GraphicsSubmitInfo> const &submissions, GhaFence const *signalFence) {
		@autoreleasepool {
			for(auto const &submission : submissions) {
				for(auto const &commandBuffer : submission.commandBuffers) {
					id<MTLCommandBuffer> mtlCommandBuffer{ [commandQueue commandBuffer] };
					polyCast<MetalGraphicsCommandBuffer>(commandBuffer.get())->executeCommands(mtlCommandBuffer);
					
					//TODO: Encoders signal semaphores AND wait on them
				}
			}
			
			if(signalFence != nullptr) {
				//Create a separate command buffer to enqueue at the end that signals the semaphore
				id<MTLCommandBuffer> mtlCommandBuffer{ [commandQueue commandBuffer] };
				
				__block dispatch_semaphore_t block_semaphore{ polyCast<MetalFence const>(signalFence)->getSemaphore() };
				[mtlCommandBuffer addCompletedHandler:^(id<MTLCommandBuffer> buffer) {
					 dispatch_semaphore_signal(block_semaphore);
				 }];
			}
		}
	}
}
