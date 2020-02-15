#pragma once

#include "Clove/Graphics/Core/Surface.hpp"

#include <wrl.h>

struct ID3D11Device;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;

namespace clv::gfx::d3d{
	class D3DRenderTarget;
}

namespace clv::gfx::d3d{
	class D3DSurface : public Surface{
		//VARIABLES
	public:
		utl::SingleCastDelegate<void()> onDeviceRemoved;

	private:
		uint32_t bufferCount = 1u;

		Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
		uint32_t swapInterval = 1u;

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;

		std::shared_ptr<D3DRenderTarget> renderTarget;
		std::optional<mth::vec2ui> desiredBufferSize;

		//FUNCTIONS
	public:
		D3DSurface() = delete;
		D3DSurface(ID3D11Device& d3dDevice, void* windowData);

		D3DSurface(const D3DSurface& other) = delete;
		D3DSurface(D3DSurface&& other) noexcept;

		D3DSurface& operator=(const D3DSurface& other) = delete;
		D3DSurface& operator=(D3DSurface&& other) noexcept;

		virtual ~D3DSurface();

		virtual void setVSync(bool vsync) override;
		virtual bool isVsync() const override;

		virtual void resizeBuffers(const mth::vec2ui& size) override;

		virtual void makeCurrent() override{};

		virtual void present() override;

		virtual std::shared_ptr<RenderTarget> getRenderTarget() const override;

		Microsoft::WRL::ComPtr<IDXGISwapChain> getSwapChain() const;
	};
}
