#include "Clove/Graphics/Metal/MetalComputeCommandBuffer.hpp"

#include "Clove/Graphics/Metal/MetalBuffer.hpp"
#include "Clove/Graphics/Metal/MetalComputePipelineObject.hpp"
#include "Clove/Graphics/Metal/MetalDescriptorSet.hpp"
#include "Clove/Graphics/Metal/MetalGlobals.hpp"
#include "Clove/Graphics/Metal/MetalImage.hpp"

#include <Clove/Cast.hpp>

namespace garlic::clove {
	MetalComputeCommandBuffer::MetalComputeCommandBuffer() = default;
	
	MetalComputeCommandBuffer::MetalComputeCommandBuffer(MetalComputeCommandBuffer &&other) noexcept = default;
	
	MetalComputeCommandBuffer &MetalComputeCommandBuffer::operator=(MetalComputeCommandBuffer &&other) noexcept = default;
	
	MetalComputeCommandBuffer::~MetalComputeCommandBuffer() = default;

	void MetalComputeCommandBuffer::beginRecording(CommandBufferUsage usageFlag) {
		commands.clear();
	}
	
	void MetalComputeCommandBuffer::endRecording() {
		//no op
	}

	void MetalComputeCommandBuffer::bindPipelineObject(GhaComputePipelineObject &pipelineObject) {
		commands.emplace_back([pipelineObject = &pipelineObject](id<MTLComputeCommandEncoder> encoder){
			[encoder setComputePipelineState:polyCast<MetalComputePipelineObject>(pipelineObject)->getPipelineState()];
		});
	}

	void MetalComputeCommandBuffer::bindDescriptorSet(GhaDescriptorSet &descriptorSet, uint32_t const setNum) {
		commands.emplace_back([descriptorSet = &descriptorSet, setNum](id<MTLComputeCommandEncoder> encoder){
			[encoder setBuffer:polyCast<MetalDescriptorSet>(descriptorSet)->getComputeBuffer()
						offset:0
					   atIndex:setNum];
		});
	}
	
	void MetalComputeCommandBuffer::pushConstant(size_t const offset, size_t const size, void const *data) {
		uint8_t *cachedData{ reinterpret_cast<uint8_t*>(malloc(size)) };
		memcpy(cachedData, data, size);
		
		commands.emplace_back([size, data = cachedData](id<MTLComputeCommandEncoder> encoder){
			[encoder setBytes:data
					   length:size
					  atIndex:pushConstantSlot];
			
			delete data;
		});
	}

	void MetalComputeCommandBuffer::disptach(vec3ui groupCount) {
		commands.emplace_back([groupCount](id<MTLComputeCommandEncoder> encoder){
			//TODO: Currently all sizes are hard coded as 256. The threads per group is something that's set inside the shader in DX and Vulkan so there needs to be shader reflection to retrieve it.
			uint32_t constexpr workGroupSize{ 256 };
			
			uint32_t numGroups{ 1 };
			if(groupCount.y > 1) {
				++numGroups;
			}
			if(groupCount.z > 1 ) {
				++numGroups;
			}
			
			vec3ui const workGroup{
				workGroupSize / numGroups,
				groupCount.y > 1 ? workGroupSize / numGroups : 1,
				groupCount.z > 1 ? workGroupSize / numGroups : 1
			};
			
			[encoder dispatchThreadgroups:MTLSizeMake(groupCount.x, groupCount.y, groupCount.z)
					threadsPerThreadgroup:MTLSizeMake(workGroup.x, workGroup.y, workGroup.z)];
		});
	}

	void MetalComputeCommandBuffer::bufferMemoryBarrier(GhaBuffer &buffer, BufferMemoryBarrierInfo const &barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage) {
		commands.emplace_back([buffer = &buffer, sourceStage, destinationStage](id<MTLComputeCommandEncoder> encoder){
			id<MTLBuffer> mtlBuffer{ polyCast<MetalBuffer>(buffer)->getBuffer() };
			[encoder memoryBarrierWithResources:&mtlBuffer
										  count:1];
		});
	}
	
	void MetalComputeCommandBuffer::imageMemoryBarrier(GhaImage &image, ImageMemoryBarrierInfo const &barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage) {
		commands.emplace_back([image = &image, sourceStage, destinationStage](id<MTLComputeCommandEncoder> encoder){
			id<MTLTexture> mtlTexture{ polyCast<MetalImage>(image)->getTexture() };
			[encoder memoryBarrierWithResources:&mtlTexture
										  count:1];
		});
	}
}
