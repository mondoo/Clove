#include "Clove/Graphics/Metal/MetalGraphicsQueue.hpp"

#include "Clove/Graphics/Metal/MetalGraphicsCommandBuffer.hpp"
#include "Clove/Graphics/Metal/MetalView.hpp"
#include "Clove/Graphics/Metal/MetalFence.hpp"
#include "Clove/Graphics/Metal/MetalPipelineObject.hpp"
#include "Clove/Graphics/Metal/MetalSemaphore.hpp"

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
					
					for(auto &pass : polyCast<MetalGraphicsCommandBuffer>(commandBuffer.get())->getEncodedRenderPasses()) {
						id<MTLRenderCommandEncoder> encoder{ pass.begin(mtlCommandBuffer) };
						
						//Inject the wait semaphore into each buffer
						for (auto const &semaphore : submission.waitSemaphores) {
							auto *metalSemaphore{ polyCast<MetalSemaphore const>(semaphore.first.get()) };
							MTLRenderStages const waitStage{ convertStage(semaphore.second) };
							
							[encoder waitForFence:metalSemaphore->getFence()
									 beforeStages:waitStage];
						}
						
						//Excute all recorded commands for the encoder
						for(auto &command : pass.commands) {
							command(encoder);
						}
					}
					
					[mtlCommandBuffer commit];
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
