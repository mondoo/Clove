#include "clvpch.hpp"
#include "DXContext.hpp"

#include "Graphics/DirectX-11/DX11Exception.hpp"
#include "Platform/Windows/WindowsWindow.hpp"

#include <d3d11.h>

namespace clv::gfx{
	DXContext::DXContext(DXContext&& other) noexcept = default;

	DXContext& DXContext::operator=(DXContext&& other) noexcept = default;

	DXContext::~DXContext() = default;

	DXContext::DXContext(void* windowData){
		clv::WindowsData* data = reinterpret_cast<clv::WindowsData*>(windowData);

		DXGI_SWAP_CHAIN_DESC scd = { 0 };
		scd.BufferDesc.Width = data->width;
		scd.BufferDesc.Height = data->height;
		scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		scd.BufferDesc.RefreshRate.Numerator = 0;
		scd.BufferDesc.RefreshRate.Denominator = 0;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = data->handle;
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

		//Get access to the texture subresource (back buffer)
		Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
		DX11_THROW_INFO(swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
		DX11_THROW_INFO(d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &target));

		//Create depth stencil state
		D3D11_DEPTH_STENCIL_DESC depthDesc = {};
		depthDesc.DepthEnable = TRUE;
		depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDesc.DepthFunc = D3D11_COMPARISON_LESS;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsstate;
		DX11_THROW_INFO(d3dDevice->CreateDepthStencilState(&depthDesc, &dsstate));

		//Bind depth state
		d3dContext->OMSetDepthStencilState(dsstate.Get(), 1u);

		//Create depth stencil texture
		D3D11_TEXTURE2D_DESC depthTexDesc = {};
		depthTexDesc.Width = data->width;
		depthTexDesc.Height = data->height;
		depthTexDesc.MipLevels = 1u;
		depthTexDesc.ArraySize = 1u;
		depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT; //D for depth
		depthTexDesc.SampleDesc.Count = 1u;
		depthTexDesc.SampleDesc.Quality = 0u;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;
		DX11_THROW_INFO(d3dDevice->CreateTexture2D(&depthTexDesc, nullptr, &depthStencil));

		//Create view of depth stencil texture
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0u;

		DX11_THROW_INFO(d3dDevice->CreateDepthStencilView(depthStencil.Get(), &dsvDesc, &dsv));

		//Bind depth stencil view to output merger
		d3dContext->OMSetRenderTargets(1u, target.GetAddressOf(), dsv.Get());

		//Set primitive topology to triangle list (groups of 3 verticies)
		d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//Configure viewport (maps the render space to an area on screen)
		D3D11_VIEWPORT vp = { 0 };
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		vp.Width = data->width;
		vp.Height = data->height;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		d3dContext->RSSetViewports(1u, &vp);

		//resterizer state
		D3D11_RASTERIZER_DESC rdesc = {};
		rdesc.FillMode = D3D11_FILL_SOLID;
		rdesc.CullMode = D3D11_CULL_BACK;
		rdesc.FrontCounterClockwise = TRUE; //We need to set the front face to CCW to be compatable with opengl/glm

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rstate;
		DX11_THROW_INFO(d3dDevice->CreateRasterizerState(&rdesc, &rstate));

		d3dContext->RSSetState(rstate.Get());

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

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> DXContext::getTarget() const{
		return target;
	}

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DXContext::getDSV() const{
		return dsv;
	}

	void DXContext::makeCurrent(){
		//Empty
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