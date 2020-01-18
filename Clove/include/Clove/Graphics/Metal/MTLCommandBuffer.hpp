#pragma once

#include "Clove/Graphics/Core/CommandBuffer.hpp"

namespace clv::gfx::mtl{
	class MTLRenderTarget;
}

namespace clv::gfx::mtl{
	class MTLCommandBuffer : public CommandBuffer{
		//VARIABLES
	private:
		id<MTLCommandQueue> commandQueue = nullptr;
		id<MTLCommandBuffer> commandBuffer = nullptr;
		id<MTLRenderCommandEncoder> commandEncoder = nullptr;
		
		id<MTLBuffer> indexBuffer = nullptr;
		
		std::shared_ptr<MTLRenderTarget> mtlRenderTarget;
		
		MTKView* view = nullptr;
		id<MTLDrawable> drawable = nullptr;
		
		//FUNCTIONS
	public:
		MTLCommandBuffer() = delete;
		MTLCommandBuffer(id<MTLCommandQueue> commandQueue, const std::shared_ptr<RenderTarget>& renderTarget);
		MTLCommandBuffer(id<MTLCommandQueue> commandQueue, Surface& surface);
		
		MTLCommandBuffer(const MTLCommandBuffer& other) = delete;
		MTLCommandBuffer(MTLCommandBuffer&& other) noexcept;

		MTLCommandBuffer& operator=(const MTLCommandBuffer& other) = delete;
		MTLCommandBuffer& operator=(MTLCommandBuffer&& other) noexcept;

		virtual ~MTLCommandBuffer();

		virtual void beginEncoding() override;

		virtual void bindIndexBuffer(const Buffer& buffer) override;
		virtual void bindVertexBuffer(const Buffer& buffer, const uint32 stride) override;
		virtual void bindShaderResourceBuffer(const Buffer& buffer, const ShaderType shaderType, const uint32 bindingPoint) override;
		virtual void bindPipelineObject(const PipelineObject& pipelineObject) override;
		virtual void bindTexture(const Texture* texture, const uint32 bindingPoint) override;
		
		virtual void setViewport(const Viewport& viewport) override;
		virtual void setDepthEnabled(bool enabled) override;

		virtual void drawIndexed(const uint32 count) override;

		virtual void flushCommands() override;
	};
}
