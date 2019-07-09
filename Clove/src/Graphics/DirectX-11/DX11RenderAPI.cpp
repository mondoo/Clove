#include "clvpch.hpp"
#include "DX11RenderAPI.hpp"

#include "Graphics/DirectX-11/DXContext.hpp"
#include "Graphics/DirectX-11/DX11Exception.hpp"
#include "Graphics/DirectX-11/Bindables/DX11IndexBuffer.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>

namespace clv::gfx{
	DX11RenderAPI::~DX11RenderAPI() = default;

	DX11RenderAPI::DX11RenderAPI(const Context& context){
		if(const DXContext* dxCon = dynamic_cast<const DXContext*>(&context)){
			d3dDevice = dxCon->getDevice();
			d3dContext = dxCon->getContext();
			target = dxCon->getTarget();
			dsv = dxCon->getDSV();
		}
	}

	void DX11RenderAPI::clear(){
		d3dContext->ClearRenderTargetView(target.Get(), math::valuePtr(clearColour));
		d3dContext->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0xff);
	}

	void DX11RenderAPI::drawIndexed(const unsigned int count){
		DX11_THROW_INFO_ONLY(d3dContext->DrawIndexed(static_cast<UINT>(count), 0u, 0u));
	}

	void DX11RenderAPI::setClearColour(const math::Vector4f& colour){
		clearColour = colour;
	}

	void DX11RenderAPI::setIndexBuffer(const IndexBuffer& buffer){
		const DX11IndexBuffer& dxBuff = static_cast<const DX11IndexBuffer&>(buffer);
		d3dContext->IASetIndexBuffer(dxBuff.getBuffer(), DXGI_FORMAT_R32_UINT, 0u);
	}

	/*ID3D11Device& DX11RenderAPI::getDevice() const{
		CLV_ASSERT(d3dDevice != nullptr, __FUNCTION__" called with null device");
		return *d3dDevice.Get();
	}*/

	/*ID3D11DeviceContext& DX11RenderAPI::getContext() const{
		CLV_ASSERT(d3dDevice != nullptr, __FUNCTION__" called with null context");
		return *d3dContext.Get();
	}*/

#if CLV_DEBUG
	DXGIInfoManager& DX11RenderAPI::getInfoManager(){
		return infoManager;
	}
#endif
}