#include "Clove/Graphics/Metal/MetalTransferQueue.hpp"

#include "Clove/Graphics/Metal/MetalPipelineObject.hpp"
#include "Clove/Graphics/Metal/MetalTransferCommandBuffer.hpp"
#include "Clove/Graphics/Metal/MetalSemaphore.hpp"
#include "Clove/Graphics/Metal/MetalFence.hpp"

#include <Clove/Cast.hpp>

namespace garlic::clove {
	MetalTransferQueue::MetalTransferQueue(id<MTLCommandQueue> commandQueue)
		: commandQueue{ [commandQueue retain] } {
	}
	
	MetalTransferQueue::MetalTransferQueue(MetalTransferQueue &&other) noexcept = default;
	
	MetalTransferQueue& MetalTransferQueue::operator=(MetalTransferQueue &&other) noexcept = default;
	
	MetalTransferQueue::~MetalTransferQueue() {
		[commandQueue release];
	}

	std::unique_ptr<GhaTransferCommandBuffer> MetalTransferQueue::allocateCommandBuffer() {
		return std::make_unique<MetalTransferCommandBuffer>();
	}
	
	void MetalTransferQueue::freeCommandBuffer(GhaTransferCommandBuffer &buffer) {
		//no op
	}

	void MetalTransferQueue::submit(std::vector<TransferSubmitInfo> const &submissions, GhaFence const *signalFence) {
		@autoreleasepool {
			for(auto const &submission : submissions) {
				for(auto const &commandBuffer : submission.commandBuffers) {
					id<MTLCommandBuffer> executionBuffer{ [commandQueue commandBuffer] };
					id<MTLBlitCommandEncoder> encoder{ [executionBuffer blitCommandEncoder] };
					auto *metalTransferBuffer{ polyCast<MetalTransferCommandBuffer>(commandBuffer.get()) };
					
					//Inject the wait semaphore into each buffer
					for (auto const &semaphore : submission.waitSemaphores) {
						auto *metalSemaphore{ polyCast<MetalSemaphore const>(semaphore.first.get()) };
						
						[encoder waitForFence:metalSemaphore->getFence()];
					}
					
					//Excute all recorded commands for the encoder
					for(auto &command : metalTransferBuffer->getCommands()) {
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
