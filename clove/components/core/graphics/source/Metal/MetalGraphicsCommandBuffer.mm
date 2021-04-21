#include "Clove/Graphics/Metal/MetalGraphicsCommandBuffer.hpp"

#include "Clove/Graphics/Metal/MetalFramebuffer.hpp"
#include "Clove/Graphics/Metal/MetalGraphicsPipelineObject.hpp"
#include "Clove/Graphics/Metal/MetalBuffer.hpp"
#include "Clove/Graphics/Metal/MetalRenderPass.hpp"

#include <Clove/Cast.hpp>

namespace garlic::clove {
	namespace {
		MTLIndexType getIndexType(IndexType garlicType) {
			switch(garlicType) {
				case IndexType::Uint16:
					return MTLIndexTypeUInt16;
				default:
					CLOVE_ASSERT(false, "{0}: Unkown index type", CLOVE_FUNCTION_NAME);
					return MTLIndexTypeUInt16;
			}
		}
	}
	
	MetalGraphicsCommandBuffer::MetalGraphicsCommandBuffer(id<MTLCommandBuffer> commandBuffer)
		: commandBuffer{ commandBuffer }{
	}
	
	MetalGraphicsCommandBuffer::MetalGraphicsCommandBuffer(MetalGraphicsCommandBuffer &&other) noexcept = default;
	
	MetalGraphicsCommandBuffer& MetalGraphicsCommandBuffer::operator=(MetalGraphicsCommandBuffer &&other) noexcept = default;
	
	MetalGraphicsCommandBuffer::~MetalGraphicsCommandBuffer() {
		//TODO: Might not be required
		//[commandBuffer release];
	}
	
	void MetalGraphicsCommandBuffer::beginRecording(CommandBufferUsage usageFlag) {
		//no op
	}
	
	void MetalGraphicsCommandBuffer::endRecording() {
		//no op
	}

	void MetalGraphicsCommandBuffer::beginRenderPass(GhaRenderPass &renderPass, GhaFramebuffer &frameBuffer, RenderArea const &renderArea, std::span<ClearValue> clearValues) {
		GhaRenderPass::Descriptor const &renderPassDescriptor{ polyCast<MetalRenderPass>(&renderPass)->getDescriptor() };
		
		//Modify the attachments to have the correct clear values.
		MTLRenderPassDescriptor *frameBufferDescriptor{ polyCast<MetalFramebuffer>(&frameBuffer)->getRenderPassDescriptor() };
		for(size_t i{ 0 }; i < renderPassDescriptor.colourAttachments.size(); ++i) {
			ColourValue const colour{ std::get<ColourValue>(clearValues[i]) };
			frameBufferDescriptor.colorAttachments[i].clearColor = MTLClearColor{ colour.r, colour.g, colour.b, colour.a };
		}
		DepthStencilValue const depthStencilValue{ std::get<DepthStencilValue>(clearValues.back()) };
		frameBufferDescriptor.depthAttachment.clearDepth = depthStencilValue.depth;
		
		//TODO: RenderArea
		
		currentEncoder = [commandBuffer renderCommandEncoderWithDescriptor:frameBufferDescriptor];
	}
	
	void MetalGraphicsCommandBuffer::endRenderPass() {
		[currentEncoder endEncoding];
	}

	void MetalGraphicsCommandBuffer::setViewport(vec2i position, vec2ui size) {
		MTLViewport viewport {
			.originX = static_cast<double>(position.x),
			.originY = static_cast<double>(position.y),
			.width   = static_cast<double>(size.x),
			.height  = static_cast<double>(size.y),
		};
		[currentEncoder setViewport:std::move(viewport)];
	}
	
	void MetalGraphicsCommandBuffer::setScissor(vec2i position, vec2ui size) {
		MTLScissorRect scissorRect {
			.x      = static_cast<NSUInteger>(position.x),
			.y      = static_cast<NSUInteger>(position.y),
			.width  = size.x,
			.height = size.y,
		};
		[currentEncoder setScissorRect:std::move(scissorRect)];
	}

	void MetalGraphicsCommandBuffer::bindPipelineObject(GhaGraphicsPipelineObject &pipelineObject) {
		[currentEncoder setRenderPipelineState:polyCast<MetalGraphicsPipelineObject>(&pipelineObject)->getPipeline()];
	}
	
	void MetalGraphicsCommandBuffer::bindVertexBuffer(GhaBuffer &vertexBuffer, size_t const offset) {
		[currentEncoder setVertexBuffer:polyCast<MetalBuffer>(&vertexBuffer)->getBuffer()
								 offset:offset
								atIndex:0];
	}
	
	void MetalGraphicsCommandBuffer::bindIndexBuffer(GhaBuffer &indexBuffer, size_t const offset, IndexType indexType) {
		if(cachedIndexBuffer.buffer != nullptr) {
			[cachedIndexBuffer.buffer release];
		}
		
		cachedIndexBuffer.buffer = [polyCast<MetalBuffer>(&indexBuffer)->getBuffer() retain];
		cachedIndexBuffer.indexType = getIndexType(indexType);
		cachedIndexBuffer.offset = offset;
	}
	
	void MetalGraphicsCommandBuffer::bindDescriptorSet(GhaDescriptorSet &descriptorSet, uint32_t const setNum) {
		//TODO
	}
	
	void MetalGraphicsCommandBuffer::pushConstant(GhaShader::Stage const stage, size_t const offset, size_t const size, void const *data) {
		//TODO
	}

	void MetalGraphicsCommandBuffer::drawIndexed(size_t const indexCount) {
		[currentEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle indexCount:indexCount indexType:cachedIndexBuffer.indexType indexBuffer:cachedIndexBuffer.buffer indexBufferOffset:cachedIndexBuffer.offset];
	}

	void MetalGraphicsCommandBuffer::bufferMemoryBarrier(GhaBuffer &buffer, BufferMemoryBarrierInfo const &barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage) {
		//TODO
	}
	
	void MetalGraphicsCommandBuffer::imageMemoryBarrier(GhaImage &image, ImageMemoryBarrierInfo const &barrierInfo, PipelineStage sourceStage, PipelineStage destinationStage) {
		//TODO
	}
	
	id<MTLCommandBuffer> MetalGraphicsCommandBuffer::getCommandBuffer() const {
		return commandBuffer;
	}
}
