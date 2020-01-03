#include "Clove/Graphics/Metal/MTLRenderDevice.hpp"

#include "Clove/Graphics/Metal/Resources/MTLBuffer.hpp"
#include "Clove/Graphics/Metal/Resources/MTLTexture.hpp"
#include "Clove/Graphics/Metal/MTLSurface.hpp"
#include "Clove/Graphics/Metal/MTLPipelineObject.hpp"
#include "Clove/Graphics/Metal/MTLRenderTarget.hpp"

namespace clv::gfx::mtl{
	MTLRenderDevice::MTLRenderDevice(id<MTLCommandQueue> commandQueue){
		this->commandQueue = [commandQueue retain];
		commandBuffer = [commandQueue commandBuffer];
	}
	
	MTLRenderDevice::~MTLRenderDevice(){
		[commandQueue release];
		[commandBuffer release];
		[commandEncoder release];
		[drawable release];
	}
	
	void MTLRenderDevice::bindIndexBuffer(const Buffer& buffer){
		const MTLBuffer& mtlBuffer = static_cast<const MTLBuffer&>(buffer);
		indexBuffer = mtlBuffer.getMTLBuffer();
	}
	
	void MTLRenderDevice::bindVertexBuffer(const Buffer& buffer, const uint32 stride){
		const MTLBuffer& mtlBuffer = static_cast<const MTLBuffer&>(buffer);
		[commandEncoder setVertexBuffer:mtlBuffer.getMTLBuffer() offset:0 atIndex:0];
	}
	
	void MTLRenderDevice::bindShaderResourceBuffer(const Buffer& buffer, const ShaderType shaderType, const uint32 bindingPoint){
		const MTLBuffer& mtlBuffer = static_cast<const MTLBuffer&>(buffer);
		switch(shaderType){
			case ShaderType::Vertex:
				[commandEncoder setVertexBuffer:mtlBuffer.getMTLBuffer() offset:0 atIndex:bindingPoint];
				break;

			case ShaderType::Pixel:
				[commandEncoder setFragmentBuffer:mtlBuffer.getMTLBuffer() offset:0 atIndex:bindingPoint];
				break;

			default:
				CLV_ASSERT(false, "Unhandled shader type in {0}", CLV_FUNCTION_NAME);
				break;
		}
	}
	
	void MTLRenderDevice::bindPipelineObject(const PipelineObject& pipelineObject){
		const MTLPipelineObject& mtlPipelineObject = static_cast<const MTLPipelineObject&>(pipelineObject);
		[commandEncoder setRenderPipelineState:mtlPipelineObject.getMTLPipelineState()];
	}
	
	void MTLRenderDevice::bindTexture(const Texture* texture, const uint32 bindingPoint){
		const MTLTexture* mtlTexture = static_cast<const MTLTexture*>(texture);
		if (mtlTexture){
			[commandEncoder setFragmentTexture:mtlTexture->getMTLTexture() atIndex:bindingPoint];
		} else{
			[commandEncoder setFragmentTexture:nullptr atIndex:bindingPoint];
		}
	}
	
	void MTLRenderDevice::updateBufferData(Buffer& buffer, const void* data){
		MTLBuffer& mtlBuffer = static_cast<MTLBuffer&>(buffer);
		memcpy([mtlBuffer.getMTLBuffer() contents], data, mtlBuffer.getDescriptor().bufferSize);
	}

	void MTLRenderDevice::makeSurfaceCurrent(const std::shared_ptr<Surface>& surface){
		currentSurface = std::static_pointer_cast<MTLSurface>(surface);
		
		//TODO: Temp from resetRenderTargetToDefault
		MTKView* view = currentSurface->getView();
		
		MTLRenderPassDescriptor* descriptor = [view currentRenderPassDescriptor];
		commandEncoder = [commandBuffer renderCommandEncoderWithDescriptor:descriptor];
		drawable = [[view currentDrawable] retain];
	}

	void MTLRenderDevice::setRenderTarget(const RenderTarget* renderTarget){
		//TODO: Can't do these operations while encoding (has to end first)
		/*const MTLRenderTarget* mtlRenderTarget = static_cast<const MTLRenderTarget*>(renderTarget);
		if (mtlRenderTarget){
			commandEncoder = [commandBuffer renderCommandEncoderWithDescriptor:mtlRenderTarget->getRenderPassDescriptor()];
		}else{
			[commandEncoder release];
		}*/
	}
	
	void MTLRenderDevice::resetRenderTargetToDefault(){
		//TODO: Can't do these operations while encoding (has to end first)
		/*MTKView* view = currentSurface->getView();
		
		MTLRenderPassDescriptor* descriptor = [view currentRenderPassDescriptor];
		commandEncoder = [commandBuffer renderCommandEncoderWithDescriptor:descriptor];
		drawable = [[view currentDrawable] retain];*/
	}

	void MTLRenderDevice::setViewport(const Viewport& viewport){
		MTLViewport vp{};
		vp.originX 	= viewport.x;
		vp.originY 	= viewport.y;
		vp.width 	= viewport.width;
		vp.height 	= viewport.height;
		vp.znear 	= 0.0;
		vp.zfar 	= 1.0;
		
		[commandEncoder setViewport:vp];
	}

	void MTLRenderDevice::clear(){
		//TODO: Putting this in clear for now but it needs to go in a commandBuffer class
		
		[commandEncoder endEncoding];
		[commandBuffer presentDrawable:drawable];
		[commandBuffer commit];
		
		//TODO: Temp - this is done to start a new command session?
		commandBuffer = [commandQueue commandBuffer];
		
		//Start up a new encoding session
		MTKView* view = currentSurface->getView();
		
		MTLRenderPassDescriptor* descriptor = [view currentRenderPassDescriptor];
		commandEncoder = [commandBuffer renderCommandEncoderWithDescriptor:descriptor];
		drawable = [[view currentDrawable] retain];
	}
	
	void MTLRenderDevice::drawIndexed(const uint32 count){
		[commandEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
								   indexCount:count
									indexType:MTLIndexTypeUInt32
								  indexBuffer:indexBuffer
							indexBufferOffset:0];
	}

	void MTLRenderDevice::setClearColour(const mth::vec4f& colour){
		//TODO: is done on the view
	}
	
	void MTLRenderDevice::setDepthBuffer(bool enabled){
		//TODO: Need to get the current descriptor used and modify that (like d3d)
		
		/*id<MTLDevice> mtlDevice = commandEncoder.device;
		
		MTLDepthStencilDescriptor* descriptor = [[MTLDepthStencilDescriptor alloc] init];
		[descriptor setDepthWriteEnabled:enabled];
		
		id<MTLDepthStencilState> state = [mtlDevice newDepthStencilStateWithDescriptor:descriptor];
		
		[commandEncoder setDepthStencilState:state];
		
		[descriptor release];*/
	}
	
	void MTLRenderDevice::setBlendState(bool enabled){
		//TODO: This needs to be set up on the pipeline object
	}
}
