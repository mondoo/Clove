#pragma once

#include "Clove/Graphics/CommandBuffer.hpp"

#import <MetalKit/MetalKit.h>

namespace clv::gfx::mtl{
	class MTLRenderTarget;
}

namespace clv::gfx::mtl{
	class MTLCommandBuffer : public CommandBuffer{
		//VARIABLES
	private:
		std::shared_ptr<GraphicsFactory> factory;

		id<MTLCommandQueue> commandQueue = nullptr;
		id<MTLCommandBuffer> commandBuffer = nullptr;
		id<MTLRenderCommandEncoder> commandEncoder = nullptr;

		id<MTLBuffer> indexBuffer = nullptr;

		std::shared_ptr<MTLRenderTarget> mtlRenderTarget;

		//FUNCTIONS
	public:
		MTLCommandBuffer() = delete;
		MTLCommandBuffer(std::shared_ptr<GraphicsFactory> factory, id<MTLCommandQueue> commandQueue);

		MTLCommandBuffer(const MTLCommandBuffer& other) = delete;
		MTLCommandBuffer(MTLCommandBuffer&& other) noexcept;

		MTLCommandBuffer& operator=(const MTLCommandBuffer& other) = delete;
		MTLCommandBuffer& operator=(MTLCommandBuffer&& other) noexcept;

		~MTLCommandBuffer();

		const std::shared_ptr<GraphicsFactory>& getFactory() const override;

		void beginEncoding(const std::shared_ptr<RenderTarget>& renderTarget) override;

		void clearTarget() override{}

		void updateBufferData(Buffer& buffer, const void* data) override;

		void bindIndexBuffer(const Buffer& buffer) override;
		void bindVertexBuffer(const Buffer& buffer, const uint32 stride) override;
		void bindShaderResourceBuffer(const Buffer& buffer, const ShaderStage shaderType, const uint32 bindingPoint) override;
		void bindPipelineObject(const PipelineObject& pipelineObject) override;
		void bindTexture(const Texture* texture, const uint32 bindingPoint) override;
		
		void setViewport(const Viewport& viewport) override;
		void setDepthEnabled(bool enabled) override;

		void drawIndexed(const uint32 count) override;

		void endEncoding() override;
	};
}
