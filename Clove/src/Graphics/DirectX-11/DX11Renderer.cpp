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
			target = dxCon->getTarget();
			dsv = dxCon->getDSV();
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