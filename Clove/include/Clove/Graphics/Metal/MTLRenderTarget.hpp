#pragma once

#include "Clove/Core/Graphics/RenderTarget.hpp"

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
		
		MTLRenderTarget(const MTLRenderTarget& other) = delete;
		MTLRenderTarget(MTLRenderTarget&& other) noexcept;
		
		MTLRenderTarget& operator=(const MTLRenderTarget& other) = delete;
		MTLRenderTarget& operator=(MTLRenderTarget&& other) noexcept;
		
		virtual ~MTLRenderTarget();
		
		MTLRenderPassDescriptor* getRenderPassDescriptor() const;
	};
}
