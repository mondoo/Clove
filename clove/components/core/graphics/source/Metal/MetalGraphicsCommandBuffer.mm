#include "Clove/Graphics/Metal/MetalGraphicsCommandBuffer.hpp"

namespace garlic::clove {
	MetalGraphicsCommandBuffer::MetalGraphicsCommandBuffer(id<MTLCommandBuffer> commandBuffer)
		: commandBuffer{ commandBuffer }{
	}
	
	MetalGraphicsCommandBuffer::MetalGraphicsCommandBuffer(MetalGraphicsCommandBuffer &&other) noexcept = default;
	
	MetalGraphicsCommandBuffer& MetalGraphicsCommandBuffer::operator=(MetalGraphicsCommandBuffer &&other) noexcept = default;
	
	MetalGraphicsCommandBuffer::~MetalGraphicsCommandBuffer() = default;
	
	void MetalGraphicsCommandBuffer::beginRecording(CommandBufferUsage usageFlag) {
		//TODO: noop?
	}
	
	void MetalGraphicsCommandBuffer::endRecording() {
		//TODO: noop?
	}

	void MetalGraphicsCommandBuffer::beginRenderPass(GhaRenderPass &renderPass, GhaFramebuffer &frameBuffer, RenderArea const &renderArea, std::span<ClearValue> clearValues) {
		//TODO: Get renderpass descriptor from frame buffer
		currentEncoder = [commandBuffer renderCommandEncoderWithDescriptor:nullptr];
	}
	
	void MetalGraphicsCommandBuffer::endRenderPass() {
		[currentEncoder endEncoding];
	}

	void MetalGraphicsCommandBuffer::setViewport(vec2i position, vec2ui size) {
		
	}
	
	void MetalGraphicsCommandBuffer::setScissor(vec2i position, vec2ui size) {
		
	}

	void MetalGraphicsCommandBuffer::bindPipelineObject(GhaGraphicsPipelineObject &pipelineObject) {
		
	}
	
	void MetalGraphicsCommandBuffer::bindVertexBuffer(GhaBuffer &vertexBuffer, size_t const offset) {
		
	}
	
	void MetalGraphicsCommandBuffer::bindIndexBuffer(GhaBuffer &indexBuffer, size_t const offset, IndexType indexType) {
		
	}
	
	void MetalGraphicsCommandBuffer::bindDescriptorSet(GhaDescriptorSet &descriptorSet, uint32_t const setNum) {
		
	}
	
	void MetalGraphicsCommandBuffer::pushConstant(GhaShader::Stage const stage, size_t const offset, size_t const size, void const *data) {
		
	}

	void MetalGraphicsCommandBuffer::drawIndexed(size_t const indexCount) {
		
	}

	void MetalGraphicsCommandBuffer::bufferMemoryBarrier(GhaBuffer &buffer, BufferMemoryBarrierInfo const &barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage) {
		
	}
	
	void MetalGraphicsCommandBuffer::imageMemoryBarrier(GhaImage &image, ImageMemoryBarrierInfo const &barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage) {
		
	}
}
