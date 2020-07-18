#include "Clove/Graphics/Metal/MTLCommandBuffer.hpp"

#include "Clove/Graphics/Metal/MTLBuffer.hpp"
#include "Clove/Graphics/Metal/MTLTexture.hpp"
#include "Clove/Graphics/Metal/MTLPipelineObject.hpp"
#include "Clove/Graphics/Metal/MTLRenderTarget.hpp"
#include "Clove/Graphics/Metal/MTLSurface.hpp"

namespace clv::gfx::mtl{
	MTLCommandBuffer::MTLCommandBuffer(std::shared_ptr<GraphicsFactory> factory, id<MTLCommandQueue> commandQueue)
		: factory(std::move(factory))
		, commandQueue([commandQueue retain]){
		commandBuffer = [[commandQueue commandBuffer] retain];
	}
	
	MTLCommandBuffer::MTLCommandBuffer(MTLCommandBuffer&& other) noexcept = default;

	MTLCommandBuffer& MTLCommandBuffer::operator=(MTLCommandBuffer&& other) noexcept = default;

	MTLCommandBuffer::~MTLCommandBuffer(){
		[commandQueue release];
		[commandBuffer release];
		[commandEncoder release];
	}

	const std::shared_ptr<GraphicsFactory>& MTLCommandBuffer::getFactory() const{
		return factory;
	}

	void MTLCommandBuffer::beginEncoding(const std::shared_ptr<RenderTarget>& renderTarget){
		if(mtlRenderTarget != nullptr){
			[commandEncoder endEncoding];
		}
		
		mtlRenderTarget = std::static_pointer_cast<MTLRenderTarget>(renderTarget);

		commandEncoder = [[commandBuffer renderCommandEncoderWithDescriptor:mtlRenderTarget->getRenderPassDescriptor()] retain];
	}

	void MTLCommandBuffer::updateBufferData(Buffer& buffer, const void* data){
		MTLBuffer& mtlBuffer = static_cast<MTLBuffer&>(buffer);
		
		mtlBuffer.updateData(data);
		
		//TODO: Ideally we wouldn't end the commandBuffer here but apprently there is no other way
		if(mtlRenderTarget != nullptr){
			[commandEncoder endEncoding];
			[commandBuffer commit];
		
			commandBuffer = [[commandQueue commandBuffer] retain];
			commandEncoder = [[commandBuffer renderCommandEncoderWithDescriptor:mtlRenderTarget->getRenderPassDescriptor()] retain];
		}
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
				GARLIC_ASSERT(false, "Unhandled shader type in {0}", GARLIC_FUNCTION_NAME);
				break;
		}
	}
	
	void MTLCommandBuffer::bindPipelineObject(const PipelineObject& pipelineObject){
		const MTLPipelineObject& mtlPipelineObject = static_cast<const MTLPipelineObject&>(pipelineObject);
		
		NSError* error = nullptr;
		id<MTLRenderPipelineState> pipelineState = [commandEncoder.device newRenderPipelineStateWithDescriptor:mtlPipelineObject.getMTLPipelineStateDescriptor() error:&error];
		if(error.code != 0){
			for (NSString* key in [error userInfo]) {
				NSString* value = [error userInfo][key];
				GARLIC_LOG(garlicLogContext, clv::Log::Level::Error, "Error in function '{0}': {1}", GARLIC_FUNCTION_NAME_PRETTY, [value cStringUsingEncoding:[NSString defaultCStringEncoding]]);
			}
			return;
		}
		
		[commandEncoder setRenderPipelineState:pipelineState];
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
