#pragma once

#include "Clove/Core/Graphics/RenderDevice.hpp"

#include "Clove/Graphics/Direct3D/DXGIInfoManager.hpp"

#include <wrl.h>

struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace clv::gfx::d3d{
	class D3DSurface;
}

namespace clv::gfx::d3d{
	class D3DRenderDevice : public RenderDevice{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext;

		std::shared_ptr<D3DSurface> currentSurface;

	#if CLV_DEBUG
		static DXGIInfoManager infoManager; //Used by certain DX11 exception macros
	#endif

		mth::vec4f clearColour = { 0.0f, 0.0f, 0.0f, 0.0f };

		//FUNCTIONS
	public:
		D3DRenderDevice() = delete;
		D3DRenderDevice(Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext);

		D3DRenderDevice(const D3DRenderDevice& other) = delete;
		D3DRenderDevice(D3DRenderDevice&& other) noexcept = delete;

		D3DRenderDevice& operator=(const D3DRenderDevice& other) = delete;
		D3DRenderDevice& operator=(D3DRenderDevice&& other) noexcept = delete;

		virtual ~D3DRenderDevice();

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

	#if CLV_DEBUG
		static DXGIInfoManager& getInfoManager();
	#endif
	};
}