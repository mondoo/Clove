#include "Clove/Graphics/Metal/MetalComputeQueue.hpp"

#include "Clove/Graphics/Metal/MetalComputeCommandBuffer.hpp"
#include "Clove/Graphics/Metal/MetalSemaphore.hpp"
#include "Clove/Graphics/Metal/MetalFence.hpp"

#include <Clove/Cast.hpp>

namespace garlic::clove {
	MetalComputeQueue::MetalComputeQueue(id<MTLCommandQueue> commandQueue)
		: commandQueue{ [commandQueue retain] } {
	}
	
	MetalComputeQueue::MetalComputeQueue(MetalComputeQueue &&other) noexcept = default;
	
	MetalComputeQueue &MetalComputeQueue::operator=(MetalComputeQueue &&other) noexcept = default;
	
	MetalComputeQueue::~MetalComputeQueue() {
		[commandQueue release];
	}

	std::unique_ptr<GhaComputeCommandBuffer> MetalComputeQueue::allocateCommandBuffer() {
		return std::make_unique<MetalComputeCommandBuffer>();
	}
	
	void MetalComputeQueue::freeCommandBuffer(GhaComputeCommandBuffer &buffer) {
		//no op
	}

	void MetalComputeQueue::submit(std::vector<ComputeSubmitInfo> const &submissions, GhaFence const *signalFence) {
		@autoreleasepool {
			for(auto const &submission : submissions) {
				for(auto const &commandBuffer : submission.commandBuffers) {
					id<MTLCommandBuffer> executionBuffer{ [commandQueue commandBuffer] };
					id<MTLComputeCommandEncoder> encoder{ [executionBuffer computeCommandEncoder] };
					auto *metalCommandBuffer{ polyCast<MetalComputeCommandBuffer>(commandBuffer.get()) };
					
					//Inject the wait semaphore into each buffer
					for (auto const &semaphore : submission.waitSemaphores) {
						auto *metalSemaphore{ polyCast<MetalSemaphore const>(semaphore.first.get()) };
						
						[encoder waitForFence:metalSemaphore->getFence()];
					}
					
					//Excute all recorded commands for the encoder
					for(auto &command : metalCommandBuffer->getCommands()) {
						command(encoder);
					}
					
					[executionBuffer commit];
				}
				
				//Signal all semaphores at the end of each submission. Similar to how vulkan does it.
				if(!submission.signalSemaphores.empty()) {
					id<MTLCommandBuffer> signalBuffer{ [commandQueue commandBuffer] };
					id<MTLBlitCommandEncoder> encoder{ [signalBuffer blitCommandEncoder] };
					
					for(auto const &semaphore : submission.signalSemaphores) {
						[encoder updateFence:polyCast<MetalSemaphore const>(semaphore.get())->getFence()];
					}
					
					[encoder endEncoding];
					[signalBuffer commit];
				}
			}
			
			//Signal fence when all submissions are completed
			if(signalFence != nullptr) {
				id<MTLCommandBuffer> signalBuffer{ [commandQueue commandBuffer] };
				
				__block dispatch_semaphore_t block_semaphore{ polyCast<MetalFence const>(signalFence)->getSemaphore() };
				[signalBuffer addCompletedHandler:^(id<MTLCommandBuffer> buffer) {
					 dispatch_semaphore_signal(block_semaphore);
				 }];
				
				[signalBuffer commit];
			}
		}
	}
}
