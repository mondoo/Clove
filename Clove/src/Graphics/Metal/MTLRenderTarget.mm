#include "Clove/Graphics/Metal/MTLRenderTarget.hpp"

#include "Clove/Graphics/Metal/Resources/MTLTexture.hpp"

namespace clv::gfx::mtl{
	MTLRenderTarget::MTLRenderTarget(id<MTLDevice> mtlDevice, Texture* colourTexture, Texture* depthStencilTexture){
		MTLTexture* mtlColourTexture = static_cast<MTLTexture*>(colourTexture);
		MTLTexture* mtldepthStencilTexture = static_cast<MTLTexture*>(depthStencilTexture);
		
		descriptor = [[MTLRenderPassDescriptor alloc] init];
		descriptor.colorAttachments[0].texture = mtlColourTexture->getMTLTexture();
		descriptor.depthAttachment.texture = mtldepthStencilTexture->getMTLTexture();
		//TODO: Not handling stencAttachment
	}

	MTLRenderTarget::MTLRenderTarget(MTLRenderPassDescriptor* descriptor)
		: descriptor(descriptor){
	}
	
	MTLRenderTarget::MTLRenderTarget(MTLRenderTarget&& other) noexcept = default;
	
	MTLRenderTarget& MTLRenderTarget::operator=(MTLRenderTarget&& other) noexcept = default;
	
	MTLRenderTarget::~MTLRenderTarget(){
		[descriptor release];
	}
	
	void MTLRenderTarget::setClearColour(const mth::vec4f& colour){
		const MTLClearColor clearColour{ colour.r, colour.g, colour.b, colour.a };
		[descriptor.colorAttachments[0] setClearColor:clearColour];
	}
	
	MTLRenderPassDescriptor* MTLRenderTarget::getRenderPassDescriptor() const{
		return descriptor;
	}
}
