#pragma once

#include "Clove/Graphics/CommandBuffer.hpp"

#include <wrl.h>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace clv::gfx::d3d{
	class D3DRenderTarget;
}

namespace clv::gfx::d3d{
	class D3DCommandBuffer : public CommandBuffer{
		//VARIABLES
	private:
		std::shared_ptr<GraphicsFactory> factory;

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> deferredContext;

		std::shared_ptr<D3DRenderTarget> d3dRenderTarget;

		//FUNCTIONS
	public:
		D3DCommandBuffer() = delete;
		D3DCommandBuffer(std::shared_ptr<GraphicsFactory> factory, ID3D11Device& d3dDevice);

		D3DCommandBuffer(const D3DCommandBuffer& other) = delete;
		D3DCommandBuffer(D3DCommandBuffer&& other) noexcept;

		D3DCommandBuffer& operator=(const D3DCommandBuffer& other) = delete;
		D3DCommandBuffer& operator=(D3DCommandBuffer&& other) noexcept;

		~D3DCommandBuffer();

		const std::shared_ptr<GraphicsFactory>& getFactory() const override;

		void beginEncoding(const std::shared_ptr<RenderTarget>& renderTarget) override;

		void clearTarget() override;

		void updateBufferData(Buffer& buffer, const void* data) override;

		void bindIndexBuffer(const Buffer& buffer) override;
		void bindVertexBuffer(const Buffer& buffer, const uint32_t stride) override;
		void bindShaderResourceBuffer(const Buffer& buffer, const ShaderStage shaderType, const uint32_t bindingPoint) override;
		void bindPipelineObject(const PipelineObject& pipelineObject) override;
		void bindTexture(const Texture* texture, const uint32_t bindingPoint) override;

		void setViewport(const Viewport& viewport) override;
		void setDepthEnabled(bool enabled) override;

		void drawIndexed(const uint32_t count) override;

		void endEncoding() override;
	};
}
