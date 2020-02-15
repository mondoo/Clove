#pragma once

#include "Clove/Graphics/Core/CommandBuffer.hpp"

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
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> deferredContext;

		std::shared_ptr<D3DRenderTarget> d3dRenderTarget;

		//FUNCTIONS
	public:
		D3DCommandBuffer() = delete;
		D3DCommandBuffer(ID3D11Device& d3dDevice);

		D3DCommandBuffer(const D3DCommandBuffer& other) = delete;
		D3DCommandBuffer(D3DCommandBuffer&& other) noexcept;

		D3DCommandBuffer& operator=(const D3DCommandBuffer& other) = delete;
		D3DCommandBuffer& operator=(D3DCommandBuffer&& other) noexcept;

		virtual ~D3DCommandBuffer();

		virtual void beginEncoding(const std::shared_ptr<RenderTarget>& renderTarget) override;

		virtual void clearTarget() override;

		virtual void updateBufferData(const Buffer& buffer, const void* data) override;

		virtual void bindIndexBuffer(const Buffer& buffer) override;
		virtual void bindVertexBuffer(const Buffer& buffer, const uint32_t stride) override;
		virtual void bindShaderResourceBuffer(const Buffer& buffer, const ShaderStage shaderType, const uint32_t bindingPoint) override;
		virtual void bindPipelineObject(const PipelineObject& pipelineObject) override;
		virtual void bindTexture(const Texture* texture, const uint32_t bindingPoint) override;

		virtual void setViewport(const Viewport& viewport) override;
		virtual void setDepthEnabled(bool enabled) override;

		virtual void drawIndexed(const uint32_t count) override;

		virtual void endEncoding() override;
	};
}