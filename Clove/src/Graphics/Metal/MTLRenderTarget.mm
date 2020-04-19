#include "Clove/Graphics/Metal/MTLRenderTarget.hpp"

#include "Clove/Graphics/Metal/MTLTexture.hpp"

namespace clv::gfx::mtl{
	MTLRenderTarget::MTLRenderTarget(std::shared_ptr<GraphicsFactory> factory, id<MTLDevice> mtlDevice, Texture* colourTexture, Texture* depthStencilTexture)
		: factory(std::move(factory)) {
		MTLTexture* mtlColourTexture = static_cast<MTLTexture*>(colourTexture);
		MTLTexture* mtldepthStencilTexture = static_cast<MTLTexture*>(depthStencilTexture);
		
		descriptor = [[MTLRenderPassDescriptor alloc] init];
		descriptor.colorAttachments[0].texture = mtlColourTexture->getMTLTexture();
		descriptor.depthAttachment.texture = mtldepthStencilTexture->getMTLTexture();
		//TODO: Not handling stencAttachment

		setClearColour({ 0.0f, 0.0f, 0.0f, 0.0f });
	}

	MTLRenderTarget::MTLRenderTarget(std::shared_ptr<GraphicsFactory> factory, MTLRenderPassDescriptor* descriptor)
		: factory(std::move(factory))
		, descriptor(descriptor){
		setClearColour({ 0.0f, 0.0f, 0.0f, 0.0f });
	}
	
	MTLRenderTarget::MTLRenderTarget(MTLRenderTarget&& other) noexcept = default;
	
	MTLRenderTarget& MTLRenderTarget::operator=(MTLRenderTarget&& other) noexcept = default;
	
	MTLRenderTarget::~MTLRenderTarget(){
		[descriptor release];
	}

	const std::shared_ptr<GraphicsFactory>& MTLRenderTarget::getFactory() const{
		return factory;
	}
	
	void MTLRenderTarget::setClearColour(const mth::vec4f& colour){
		const MTLClearColor clearColour{ colour.r, colour.g, colour.b, colour.a };
		[descriptor.colorAttachments[0] setClearColor:clearColour];
	}
	
	MTLRenderPassDescriptor* MTLRenderTarget::getRenderPassDescriptor() const{
		return descriptor;
	}
	
	void MTLRenderTarget::updateColourTexture(id<MTLTexture> mtlTexture){
		descriptor.colorAttachments[0].texture = mtlTexture;
	}
}
