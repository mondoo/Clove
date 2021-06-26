#include "Clove/Graphics/Metal/MetalTransferCommandBuffer.hpp"

#include "Clove/Graphics/Metal/MetalBuffer.hpp"
#include "Clove/Graphics/Metal/MetalImage.hpp"
#include "Clove/Graphics/Metal/MetalPipelineObject.hpp"

#include <Clove/Cast.hpp>

namespace garlic::clove {
	MetalTransferCommandBuffer::MetalTransferCommandBuffer() = default;
	
	MetalTransferCommandBuffer::MetalTransferCommandBuffer(MetalTransferCommandBuffer &&other) noexcept = default;
	
	MetalTransferCommandBuffer &MetalTransferCommandBuffer::operator=(MetalTransferCommandBuffer &&other) noexcept = default;
	
	MetalTransferCommandBuffer::~MetalTransferCommandBuffer() = default;

	void MetalTransferCommandBuffer::beginRecording(CommandBufferUsage usageFlag) {
		commands.clear();
	}
	
	void MetalTransferCommandBuffer::endRecording() {
	
	}

	void MetalTransferCommandBuffer::copyBufferToBuffer(GhaBuffer &source, size_t const sourceOffset, GhaBuffer &destination, size_t const destinationOffset, size_t const sizeBytes) {
		commands.emplace_back([source = &source, sourceOffset, destination = &destination, destinationOffset, sizeBytes](id<MTLBlitCommandEncoder> encoder){
			[encoder copyFromBuffer:polyCast<MetalBuffer>(source)->getBuffer()
					   sourceOffset:sourceOffset
						   toBuffer:polyCast<MetalBuffer>(destination)->getBuffer()
				  destinationOffset:destinationOffset
							   size:sizeBytes];
		});
	}
	
	void MetalTransferCommandBuffer::copyBufferToImage(GhaBuffer &source, size_t const sourceOffset, GhaImage &destination, vec3i const &destinationOffset, vec3ui const &destinationExtent) {
		commands.emplace_back([source = &source, sourceOffset, destination = &destination, destinationOffset, destinationExtent](id<MTLBlitCommandEncoder> encoder){
			[encoder copyFromBuffer:polyCast<MetalBuffer>(source)->getBuffer()
					   sourceOffset:sourceOffset
				  sourceBytesPerRow:0//Tightly packed
				sourceBytesPerImage:0//Tightly packed
						 sourceSize:MTLSizeMake(destinationExtent.x, destinationExtent.y, 1)
						  toTexture:polyCast<MetalImage>(destination)->getTexture()
				   destinationSlice:0
				   destinationLevel:0
				  destinationOrigin:MTLOriginMake(destinationOffset.x, destinationOffset.y, 0)];
		});
	}

	void MetalTransferCommandBuffer::copyImageToBuffer(GhaImage &source, vec3i const &sourceOffset, vec3ui const &sourceExtent, GhaBuffer &destination, size_t const destinationOffset) {
		commands.emplace_back([source = &source, sourceOffset, sourceExtent, destination = &destination, destinationOffset](id<MTLBlitCommandEncoder> encoder){
			[encoder copyFromTexture:polyCast<MetalImage>(source)->getTexture()
						 sourceSlice:0
						 sourceLevel:0
						sourceOrigin:MTLOriginMake(sourceOffset.x, sourceOffset.y, 0)
						  sourceSize:MTLSizeMake(sourceExtent.x, sourceExtent.y, 1)
							toBuffer:polyCast<MetalBuffer>(destination)->getBuffer()
				   destinationOffset:destinationOffset
			  destinationBytesPerRow:0//Tightliy packed
			destinationBytesPerImage:0];//Tightly packed
		});
	}

	void MetalTransferCommandBuffer::bufferMemoryBarrier(GhaBuffer &buffer, BufferMemoryBarrierInfo const &barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage) {
		//MTLBlitCommandEncoder does not have a barrier. This might not be an issue as a barrier will need to happen on the other queues for owner transfership
		//or will be synchronised with a semaphore / fence
	}
	
	void MetalTransferCommandBuffer::imageMemoryBarrier(GhaImage &image, ImageMemoryBarrierInfo const &barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage) {
		//MTLBlitCommandEncoder does not have a barrier. This might not be an issue as a barrier will need to happen on the other queues for owner transfership
		//or will be synchronised with a semaphore / fence
	}
}
