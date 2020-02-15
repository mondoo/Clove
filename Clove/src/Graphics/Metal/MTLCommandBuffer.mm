#include "Clove/Graphics/Metal/MTLCommandBuffer.hpp"

#include "Clove/Graphics/Metal/Resources/MTLBuffer.hpp"
#include "Clove/Graphics/Metal/Resources/MTLTexture.hpp"
#include "Clove/Graphics/Metal/MTLPipelineObject.hpp"
#include "Clove/Graphics/Metal/MTLRenderTarget.hpp"
#include "Clove/Graphics/Metal/MTLSurface.hpp"

namespace clv::gfx::mtl{
	MTLCommandBuffer::MTLCommandBuffer(id<MTLCommandQueue> commandQueue)
		: commandQueue([commandQueue retain]){
	}
	
	MTLCommandBuffer::MTLCommandBuffer(MTLCommandBuffer&& other) noexcept = default;

	MTLCommandBuffer& MTLCommandBuffer::operator=(MTLCommandBuffer&& other) noexcept = default;

	MTLCommandBuffer::~MTLCommandBuffer(){
		[commandQueue release];
		[commandBuffer release];
		[commandEncoder release];
	}

	void MTLCommandBuffer::beginEncoding(const std::shared_ptr<RenderTarget>& renderTarget){
		mtlRenderTarget = std::static_pointer_cast<MTLRenderTarget>(renderTarget);

		commandBuffer = [[commandQueue commandBuffer] retain];
		commandEncoder = [[commandBuffer renderCommandEncoderWithDescriptor:mtlRenderTarget->getRenderPassDescriptor()] retain];
	}

	void MTLCommandBuffer::updateBufferData(const Buffer& buffer, const void* data){
		CLV_ASSERT(false, "NOT YET IMPLEMENTED");
	}

	void MTLCommandBuffer::bindIndexBuffer(const Buffer& buffer){
		const MTLBuffer& mtlBuffer = static_cast<const MTLBuffer&>(buffer);
		indexBuffer = mtlBuffer.getMTLBuffer();
	}
	
	void MTLCommandBuffer::bindVertexBuffer(const Buffer& buffer, const uint32 stride){
		const MTLBuffer& mtlBuffer = static_cast<const MTLBuffer&>(buffer);
		[commandEncoder setVertexBuffer:mtlBuffer.getMTLBuffer() offset:0 atIndex:0];
	}
	
	void MTLCommandBuffer::bindShaderResourceBuffer(const Buffer& buffer, const ShaderStage shaderType, const uint32 bindingPoint){
		const MTLBuffer& mtlBuffer = static_cast<const MTLBuffer&>(buffer);
		switch(shaderType){
			case ShaderStage::Vertex:
				[commandEncoder setVertexBuffer:mtlBuffer.getMTLBuffer() offset:0 atIndex:bindingPoint];
				break;

			case ShaderStage::Pixel:
				[commandEncoder setFragmentBuffer:mtlBuffer.getMTLBuffer() offset:0 atIndex:bindingPoint];
				break;

			default:
				CLV_ASSERT(false, "Unhandled shader type in {0}", CLV_FUNCTION_NAME);
				break;
		}
	}
	
	void MTLCommandBuffer::bindPipelineObject(const PipelineObject& pipelineObject){
		const MTLPipelineObject& mtlPipelineObject = static_cast<const MTLPipelineObject&>(pipelineObject);
		NSError* error = nullptr;
		[commandEncoder setRenderPipelineState:[commandEncoder.device newRenderPipelineStateWithDescriptor:mtlPipelineObject.getMTLPipelineStateDescriptor() error:&error]];
		if(error.code != 0){
			for (NSString* key in [error userInfo]) {
				NSString* value = [error userInfo][key];
				CLV_LOG_ERROR("Error in function '{0}': {1}", CLV_FUNCTION_NAME_PRETTY, [value cStringUsingEncoding:[NSString defaultCStringEncoding]]);
			}
		}
		
		[commandEncoder setCullMode:mtlPipelineObject.getCullFace() == CullFace::Back ? MTLCullModeBack : MTLCullModeFront];
		[commandEncoder setFrontFacingWinding:mtlPipelineObject.isFrontFaceCounterClockWise() ? MTLWindingCounterClockwise : MTLWindingClockwise];
	}
	
	void MTLCommandBuffer::bindTexture(const Texture* texture, const uint32 bindingPoint){
		if (const MTLTexture* mtlTexture = static_cast<const MTLTexture*>(texture)){
			[commandEncoder setFragmentTexture:mtlTexture->getMTLTexture() atIndex:bindingPoint];
			[commandEncoder setFragmentSamplerState:mtlTexture->getMTLSampler() atIndex:bindingPoint];
		} else{
			[commandEncoder setFragmentTexture:nullptr atIndex:bindingPoint];
			[commandEncoder setFragmentSamplerState:nullptr atIndex:bindingPoint];
		}
	}
	
	void MTLCommandBuffer::setViewport(const Viewport& viewport){
		MTLViewport vp{};
		vp.originX 	= viewport.x;
		vp.originY 	= viewport.y;
		vp.width 	= viewport.width;
		vp.height 	= viewport.height;
		vp.znear 	= 0.0;
		vp.zfar 	= 1.0;
		
		[commandEncoder setViewport:vp];
	}
	
	void MTLCommandBuffer::setDepthEnabled(bool enabled){
		id<MTLDevice> mtlDevice = commandEncoder.device;
		
		MTLDepthStencilDescriptor* descriptor = [[MTLDepthStencilDescriptor alloc] init];
		[descriptor setDepthWriteEnabled:(enabled ? YES : NO)];
		descriptor.depthCompareFunction = MTLCompareFunctionLess;
		
		id<MTLDepthStencilState> state = [mtlDevice newDepthStencilStateWithDescriptor:descriptor];
		
		[commandEncoder setDepthStencilState:state];
		
		[descriptor release];
	}

	void MTLCommandBuffer::drawIndexed(const uint32 count){
		[commandEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
								   indexCount:count
									indexType:MTLIndexTypeUInt32
								  indexBuffer:indexBuffer
							indexBufferOffset:0];
	}

	void MTLCommandBuffer::endEncoding(){
		[commandEncoder endEncoding];
		[commandBuffer commit];
	}
}
