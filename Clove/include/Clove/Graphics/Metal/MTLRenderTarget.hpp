#pragma once

#include "Clove/Graphics/Core/RenderTarget.hpp"

#import <MetalKit/MetalKit.h>

namespace clv::gfx {
	class Texture;
}

namespace clv::gfx::mtl {
	class MTLRenderTarget : public RenderTarget {
		//VARIABLES
	private:
		std::shared_ptr<GraphicsFactory> factory;

		MTLRenderPassDescriptor* descriptor = nullptr;

		//FUNCTIONS
	public:
		MTLRenderTarget() = delete;
		MTLRenderTarget(std::shared_ptr<GraphicsFactory> factory, id<MTLDevice> mtlDevice, Texture* colourTexture, Texture* depthStencilTexture);
		MTLRenderTarget(std::shared_ptr<GraphicsFactory> factory, MTLRenderPassDescriptor* descriptor);

		MTLRenderTarget(const MTLRenderTarget& other) = delete;
		MTLRenderTarget(MTLRenderTarget&& other) noexcept;

		MTLRenderTarget& operator=(const MTLRenderTarget& other) = delete;
		MTLRenderTarget& operator=(MTLRenderTarget&& other) noexcept;

		virtual ~MTLRenderTarget();

		virtual const std::shared_ptr<GraphicsFactory>& getFactory() const override;

		virtual void clear() override {}

		virtual void setClearColour(const mth::vec4f& colour) override;

		MTLRenderPassDescriptor* getRenderPassDescriptor() const;

		void updateColourTexture(id<MTLTexture> mtlTexture);
	};
}
