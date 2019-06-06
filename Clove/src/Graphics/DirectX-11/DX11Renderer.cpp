#include "clvpch.hpp"
#include "DX11Renderer.hpp"

#include "Graphics/DirectX-11/DXContext.hpp"
#include "Graphics/DirectX-11/DX11Exception.hpp"
#include "Clove/Input/Input.hpp"
#include "Clove/Profiling/Timer.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>

namespace clv::gfx{
	DX11Renderer::~DX11Renderer() = default;

	DX11Renderer::DX11Renderer(const Context& context){
		if(const DXContext* dxCon = dynamic_cast<const DXContext*>(&context)){
			d3dDevice = dxCon->getDevice();
			d3dContext = dxCon->getContext();

			HRESULT hr;

			//Get access to the texture subresource (back buffer)
			Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
			DX11_THROW_INFO(dxCon->getSwapChain()->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
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
			depthTexDesc.Width = 1280; //TODO - need to get window width
			depthTexDesc.Height = 720; //TODO - need to get window height
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
			vp.Width = 1280; //TODO - need to get window width
			vp.Height = 720; //TODO - need to get window height
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
		}
	}

	void DX11Renderer::clear(){
		float colour[] = { 0.0f, 0.0f, 1.0f, 1.0f };
		d3dContext->ClearRenderTargetView(target.Get(), colour);
		d3dContext->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0xff);
	}

	API DX11Renderer::getAPI() const{
		return API::DirectX11;
	}

	void DX11Renderer::drawIndexed(const unsigned int count){
		CLV_TIME_SCOPE("DX11: DrawIndexed");
		HRESULT hr;
		DX11_THROW_INFO_ONLY(d3dContext->DrawIndexed(static_cast<UINT>(count), 0u, 0u));
	}
}