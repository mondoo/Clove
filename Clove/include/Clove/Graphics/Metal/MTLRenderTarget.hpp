#pragma once

#include "Clove/Graphics/Core/RenderTarget.hpp"

#import <MetalKit/MetalKit.h>

namespace clv::gfx{
	class Texture;
}

namespace clv::gfx::mtl{
	class MTLRenderTarget : public RenderTarget{
		//VARIABLES
	private:
		MTLRenderPassDescriptor* descriptor = nullptr;
		
		//FUNCTIONS
	public:
		MTLRenderTarget() = delete;
		MTLRenderTarget(id<MTLDevice> mtlDevice, Texture* colourTexture, Texture* depthStencilTexture);
		MTLRenderTarget(MTLRenderPassDescriptor* descriptor);
		
		MTLRenderTarget(const MTLRenderTarget& other) = delete;
		MTLRenderTarget(MTLRenderTarget&& other) noexcept;
		
		MTLRenderTarget& operator=(const MTLRenderTarget& other) = delete;
		MTLRenderTarget& operator=(MTLRenderTarget&& other) noexcept;
		
		virtual ~MTLRenderTarget();

		virtual void clear() override{}
		
		virtual void setClearColour(const mth::vec4f& colour);
		
		MTLRenderPassDescriptor* getRenderPassDescriptor() const;
	};
}
