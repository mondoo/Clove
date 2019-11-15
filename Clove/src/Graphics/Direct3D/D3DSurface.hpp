#pragma once

#include "Core/Graphics/Surface.hpp"

#include <wrl.h>

struct ID3D11Device;
struct IDXGISwapChain;

namespace clv::gfx::d3d{
	class D3DRenderTarget;
}

namespace clv::gfx::d3d{
	class D3DSurface : public Surface{
		//VARIABLES
	public:
		utl::SingleCastDelegate<void()> onDeviceRemoved;

	private:	
		Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
		uint32 swapInterval = 1u;

		std::unique_ptr<D3DRenderTarget> renderTarget;

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

		virtual void present() override;

		D3DRenderTarget& getTarget() const;

		Microsoft::WRL::ComPtr<IDXGISwapChain> getSwapChain() const;
	};
}
