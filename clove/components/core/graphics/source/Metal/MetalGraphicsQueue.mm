#include "Clove/Graphics/Metal/MetalGraphicsQueue.hpp"

#include "Clove/Graphics/Metal/MetalGraphicsCommandBuffer.hpp"
#include "Clove/Graphics/Metal/MetalView.hpp"

#include <Clove/Cast.hpp>

namespace garlic::clove {
	MetalGraphicsQueue::MetalGraphicsQueue(id<MTLCommandQueue> commandQueue)
		: commandQueue{ commandQueue } {
	}
	
	MetalGraphicsQueue::MetalGraphicsQueue(MetalGraphicsQueue &&other) noexcept = default;
	
	MetalGraphicsQueue& MetalGraphicsQueue::operator=(MetalGraphicsQueue &&other) noexcept = default;
	
	MetalGraphicsQueue::~MetalGraphicsQueue() = default;
	
	std::unique_ptr<GhaGraphicsCommandBuffer> MetalGraphicsQueue::allocateCommandBuffer() {
		return std::make_unique<MetalGraphicsCommandBuffer>([commandQueue commandBuffer]);
	}
	
	void MetalGraphicsQueue::freeCommandBuffer(GhaGraphicsCommandBuffer &buffer) {
		//no op
	}

	void MetalGraphicsQueue::submit(std::vector<GraphicsSubmitInfo> const &submissions, GhaFence const *signalFence) {
		for(auto &submission : submissions) {
			//TODO: Fences / semahpores
			//[currentEncoder updateFence:nullptr afterStages:MTLRenderStageFragment];
			
			for(auto &commandBuffer : submission.commandBuffers) {
				MetalGraphicsCommandBuffer *const metalGraphicsBuffer{ polyCast<MetalGraphicsCommandBuffer>(commandBuffer.get()) };
				
				id<MTLCommandBuffer> mtlCommandBuffer{ metalGraphicsBuffer->getCommandBuffer() };
				[mtlCommandBuffer commit];
				
				//Once the buffer has been committed provide a new one to record to
				metalGraphicsBuffer->setNewCommandBuffer([commandQueue commandBuffer]);
			}
		}
	}
}
