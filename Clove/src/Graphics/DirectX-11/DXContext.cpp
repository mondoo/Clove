#include "clvpch.hpp"
#include "DXContext.hpp"

#include "Graphics/DirectX-11/DX11Exception.hpp"

#include <d3d11.h>

namespace clv::gfx{
	DXContext::DXContext(DXContext&& other) noexcept = default;

	DXContext& DXContext::operator=(DXContext&& other) noexcept = default;

	DXContext::~DXContext() = default;

	DXContext::DXContext(void* windowData){
		DXGI_SWAP_CHAIN_DESC scd = { 0 };
		scd.BufferDesc.Width = 0;
		scd.BufferDesc.Height = 0;
		scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		scd.BufferDesc.RefreshRate.Numerator = 0;
		scd.BufferDesc.RefreshRate.Denominator = 0;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = reinterpret_cast<HWND>(windowData);
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = 0;

		HRESULT hr; //Used by DX11_THROW_FAILED macro

		UINT flags = 0;
	#if CLV_DEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0
		};

		DX11_THROW_INFO(D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			flags,
			featureLevels,
			static_cast<UINT>(sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL)),
			D3D11_SDK_VERSION,
			&scd,
			&swapChain,
			&d3dDevice,
			nullptr,
			&d3dContext
		));

		CLV_LOG_INFO("Successfuly created a DirectX 11 context");
	}

	Microsoft::WRL::ComPtr<ID3D11Device> DXContext::getDevice() const{
		return d3dDevice;
	}

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> DXContext::getContext() const{
		return d3dContext;
	}

	Microsoft::WRL::ComPtr<IDXGISwapChain> DXContext::getSwapChain() const{
		return swapChain;
	}

	void DXContext::present(){
		HRESULT hr;

	#if CLV_DEBUG
		infoManager.set();
	#endif
		if(FAILED(hr = swapChain->Present(1u, 0u))){
			if(hr == DXGI_ERROR_DEVICE_REMOVED){
				throw DX11_DEVICE_REMOVED_EXCPTION(d3dDevice->GetDeviceRemovedReason());
			} else{
				DX11_EXCEPT(hr);
			}
		}
	}
}