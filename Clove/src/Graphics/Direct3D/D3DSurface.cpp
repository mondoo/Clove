#include "Clove/Graphics/Direct3D/D3DSurface.hpp"

#include "Clove/Graphics/Direct3D/D3DException.hpp"
#include "Clove/Graphics/Direct3D/D3DRenderTarget.hpp"
#include "Clove/Platform/Windows/WindowsWindow.hpp"
#include "Clove/Graphics/Core/GraphicsTypes.hpp"

#include <d3d11.h>

namespace clv::gfx::d3d{
	D3DSurface::D3DSurface(ID3D11Device& d3dDevice, void* windowData){
		plt::WindowsData* data = reinterpret_cast<plt::WindowsData*>(windowData);

		DX11_INFO_PROVIDER;

		DXGI_SWAP_CHAIN_DESC swapChainDesc{};
		swapChainDesc.BufferDesc.Width						= data->width;
		swapChainDesc.BufferDesc.Height						= data->height;
		swapChainDesc.BufferDesc.Format						= DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator		= 0u;
		swapChainDesc.BufferDesc.RefreshRate.Denominator	= 0u;
		swapChainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count						= 1u;
		swapChainDesc.SampleDesc.Quality					= 0u;
		swapChainDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount							= bufferCount;
		swapChainDesc.OutputWindow							= data->handle;
		swapChainDesc.Windowed								= TRUE;
		swapChainDesc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags									= 0;

		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice = nullptr;
		DX11_THROW_INFO(d3dDevice.QueryInterface(__uuidof(IDXGIDevice), &dxgiDevice));

		Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
		DX11_THROW_INFO(dxgiDevice->GetAdapter(&dxgiAdapter));

		Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory = nullptr;
		DX11_THROW_INFO(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), &dxgiFactory));

		DX11_THROW_INFO(dxgiFactory->CreateSwapChain(&d3dDevice, &swapChainDesc, &swapChain));

		Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
		DX11_THROW_INFO(swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
		DX11_THROW_INFO(d3dDevice.CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView));

		D3D11_TEXTURE2D_DESC depthTexDesc{};
		depthTexDesc.Width				= data->width;
		depthTexDesc.Height				= data->height;
		depthTexDesc.MipLevels			= 1u;
		depthTexDesc.ArraySize			= 1u;
		depthTexDesc.Format				= DXGI_FORMAT_D32_FLOAT;
		depthTexDesc.SampleDesc.Count	= 1u;
		depthTexDesc.SampleDesc.Quality = 0u;
		depthTexDesc.Usage				= D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;

		DX11_THROW_INFO(d3dDevice.CreateTexture2D(&depthTexDesc, nullptr, &depthStencil));

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
		dsvDesc.Format				= depthTexDesc.Format;
		dsvDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice	= 0u;

		DX11_THROW_INFO(d3dDevice.CreateDepthStencilView(depthStencil.Get(), &dsvDesc, &depthStencilView));

		renderTarget = std::make_shared<D3DRenderTarget>(renderTargetView, depthStencilView);
	}

	D3DSurface::D3DSurface(D3DSurface&& other) noexcept = default;

	D3DSurface& D3DSurface::operator=(D3DSurface&& other) noexcept = default;

	D3DSurface::~D3DSurface() = default;

	void D3DSurface::setVSync(bool enabled){
		swapInterval = enabled ? 1u : 0u;
		CLV_LOG_TRACE("Swap interval for DirectX was set to: {0}", swapInterval);
	}

	bool D3DSurface::isVsync() const{
		return (swapInterval > 0u);
	}

	void D3DSurface::resizeBuffers(const mth::vec2ui& size){
		D3D11_TEXTURE2D_DESC depthTexDesc{};
		depthStencil->GetDesc(&depthTexDesc);

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
		depthStencilView->GetDesc(&dsvDesc);

		depthStencil.Reset();
		renderTargetView.Reset();
		depthStencilView.Reset();
		renderTarget.reset();

		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(swapChain->ResizeBuffers(bufferCount, size.x, size.y, DXGI_FORMAT_B8G8R8A8_UNORM, 0));

		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice;
		DX11_THROW_INFO(swapChain->GetDevice(__uuidof(ID3D11Device), &d3dDevice));

		Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
		DX11_THROW_INFO(swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
		DX11_THROW_INFO(d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView));

		depthTexDesc.Width = size.x;
		depthTexDesc.Height = size.y;

		DX11_THROW_INFO(d3dDevice->CreateTexture2D(&depthTexDesc, nullptr, &depthStencil));
		DX11_THROW_INFO(d3dDevice->CreateDepthStencilView(depthStencil.Get(), &dsvDesc, &depthStencilView));

		renderTarget = std::make_shared<D3DRenderTarget>(renderTargetView, depthStencilView);
	}

	void D3DSurface::present(){
		DX11_INFO_PROVIDER;

	#if CLV_DEBUG
		infoManager.set();
	#endif
		if(FAILED(hr = swapChain->Present(swapInterval, 0u))){
			if(hr == DXGI_ERROR_DEVICE_REMOVED){
				onDeviceRemoved.broadcast();
			} else{
				DX11_EXCEPT(hr);
			}
		}
	}

	std::shared_ptr<RenderTarget> D3DSurface::getRenderTarget() const{
		return renderTarget;
	}

	Microsoft::WRL::ComPtr<IDXGISwapChain> D3DSurface::getSwapChain() const{
		return swapChain;
	}
}