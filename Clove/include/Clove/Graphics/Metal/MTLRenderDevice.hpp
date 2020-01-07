#pragma once

#include "Clove/Graphics/Core/RenderDevice.hpp"

namespace clv::gfx::mtl{
	class MTLSurface;
}

namespace clv::gfx::mtl{
	class MTLRenderDevice : public RenderDevice{
		//VARIABLES
	private:
		id<MTLCommandQueue> commandQueue;
		id<MTLCommandBuffer> commandBuffer;
		id<MTLRenderCommandEncoder> commandEncoder;
		
		id<MTLBuffer> indexBuffer;
		
		std::shared_ptr<MTLSurface> currentSurface;
		
		//FUNCTIONS
	public:
		MTLRenderDevice() = delete;
		MTLRenderDevice(id<MTLCommandQueue> commandQueue);
		
		MTLRenderDevice(const MTLRenderDevice& other) = delete;
		MTLRenderDevice(MTLRenderDevice&& other) noexcept = delete;

		MTLRenderDevice& operator=(const MTLRenderDevice& other) = delete;
		MTLRenderDevice& operator=(MTLRenderDevice&& other) noexcept = delete;
		
		virtual ~MTLRenderDevice();

		virtual void bindIndexBuffer(const Buffer& buffer) override;
		virtual void bindVertexBuffer(const Buffer& buffer, const uint32 stride) override;
		virtual void bindShaderResourceBuffer(const Buffer& buffer, const ShaderType shaderType, const uint32 bindingPoint) override;
		virtual void bindPipelineObject(const PipelineObject& pipelineObject) override;
		virtual void bindTexture(const Texture* texture, const uint32 bindingPoint) override;
		
		virtual void updateBufferData(Buffer& buffer, const void* data) override;

		virtual void makeSurfaceCurrent(const std::shared_ptr<Surface>& surface) override;

		//Temp: adding default/clear here until I figure out the best way to handle changing the rt for the lights and then back to the surface
		virtual void setRenderTarget(const RenderTarget* renderTarget) override;
		virtual void setRenderTargetToDefault() override;
		//

		virtual void setViewport(const Viewport& viewport) override;

		virtual void clear() override;
		virtual void drawIndexed(const uint32 count) override;

		virtual void setClearColour(const mth::vec4f& colour) override;
		virtual void setDepthBuffer(bool enabled) override;
		virtual void setBlendState(bool enabled) override;
	};
}
