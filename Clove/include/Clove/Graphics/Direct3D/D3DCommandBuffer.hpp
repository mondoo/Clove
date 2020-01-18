#pragma once

#include "Clove/Graphics/Core/CommandBuffer.hpp"

#include <wrl.h>

struct ID3D11DeviceContext;

namespace clv::gfx::d3d{
	class D3DRenderTarget;
}

namespace clv::gfx::d3d{
	class D3DCommandBuffer : public CommandBuffer{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext;

		std::shared_ptr<D3DRenderTarget> d3dRenderTarget;

		std::vector<std::function<void()>> commands;

		mth::vec4f clearColour = { 0.0f, 0.0f, 0.0f, 1.0f };

		bool surfaceNeedsToResizeBuffers = false;
		utl::SingleCastDelegate<std::shared_ptr<D3DRenderTarget>()> finishBufferResize;

		//FUNCTIONS
	public:
		D3DCommandBuffer() = delete;
		D3DCommandBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, const std::shared_ptr<RenderTarget>& renderTarget);
		D3DCommandBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Surface& surface);

		D3DCommandBuffer(const D3DCommandBuffer& other) = delete;
		D3DCommandBuffer(D3DCommandBuffer&& other) noexcept;

		D3DCommandBuffer& operator=(const D3DCommandBuffer& other) = delete;
		D3DCommandBuffer& operator=(D3DCommandBuffer&& other) noexcept;

		virtual ~D3DCommandBuffer();

		virtual void beginEncoding() override;

		virtual void bindIndexBuffer(const Buffer& buffer) override;
		virtual void bindVertexBuffer(const Buffer& buffer, const uint32 stride) override;
		virtual void bindShaderResourceBuffer(const Buffer& buffer, const ShaderType shaderType, const uint32 bindingPoint) override;
		virtual void bindPipelineObject(const PipelineObject& pipelineObject) override;
		virtual void bindTexture(const Texture* texture, const uint32 bindingPoint) override;

		virtual void setViewport(const Viewport& viewport) override;
		virtual void setDepthEnabled(bool enabled) override;

		virtual void drawIndexed(const uint32 count) override;

		virtual void endEncoding() override;

	private:
		void onSurfaceBufferResizeReuqested();
	};
}