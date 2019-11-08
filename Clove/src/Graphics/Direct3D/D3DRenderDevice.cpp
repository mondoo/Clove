#include "D3DRenderDevice.hpp"

#include "Graphics/Direct3D/D3DException.hpp"
#include "Graphics/Direct3D/DX11RenderTarget.hpp"

#include <d3d11.h>

namespace clv::gfx::d3d::_11{
#if CLV_DEBUG
	DXGIInfoManager DX11RenderAPI::infoManager;
#endif

	D3DRenderDevice::D3DRenderDevice(Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext)
		: d3dContext(d3dContext){
	}

	D3DRenderDevice::~D3DRenderDevice() = default;

	void D3DRenderDevice::bindIndexBuffer(IndexBuffer& buffer){
		//TODO:
	}

	void D3DRenderDevice::bindVertexBuffer(VertexBuffer& buffer){
		//TODO:
	}

	void D3DRenderDevice::bindShaderResource(ShaderResource& resource){
		//TODO:
	}

	void D3DRenderDevice::bindTexture(Texture& texture){
		//TODO:
	}

	void D3DRenderDevice::bindShader(Shader& shader){
		//TODO:
	}

	void D3DRenderDevice::setViewport(const Viewport& viewport){
		//TODO:
	}

	void D3DRenderDevice::clear(){
		if(currentRenderTarget){
			d3dContext->ClearRenderTargetView(currentRenderTarget.Get(), math::valuePtr(clearColour));
		}
		d3dContext->ClearDepthStencilView(currentDepthStencil.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0xff);
	}

	void D3DRenderDevice::drawIndexed(const uint32 count){
		DX11_THROW_INFO_ONLY(d3dContext->DrawIndexed(static_cast<UINT>(count), 0u, 0u));
	}

	void D3DRenderDevice::setClearColour(const math::Vector4f& colour){
		clearColour = colour;
	}

	void D3DRenderDevice::setDepthBuffer(bool enabled){
		DX11_INFO_PROVIDER;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsstate;
		D3D11_DEPTH_STENCIL_DESC depthDesc = {};
		UINT stencilRef;

		d3dContext->OMGetDepthStencilState(&dsstate, &stencilRef);
		dsstate->GetDesc(&depthDesc);
		depthDesc.DepthEnable = enabled ? TRUE : FALSE;
		DX11_THROW_INFO(d3dDevice->CreateDepthStencilState(&depthDesc, &dsstate));
		d3dContext->OMSetDepthStencilState(dsstate.Get(), stencilRef);
	}

	void D3DRenderDevice::setBlendState(bool enabled){
		DX11_INFO_PROVIDER;

		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
		D3D11_BLEND_DESC blendDesc = { };
		FLOAT blendFactor[4];
		UINT sampleMask;

		d3dContext->OMGetBlendState(&blendState, blendFactor, &sampleMask);
		blendState->GetDesc(&blendDesc);
		blendDesc.RenderTarget[0].BlendEnable = enabled ? TRUE : FALSE;
		DX11_THROW_INFO(d3dDevice->CreateBlendState(&blendDesc, &blendState));
		d3dContext->OMSetBlendState(blendState.Get(), blendFactor, sampleMask);
	}

	void D3DRenderDevice::setDefaultRenderTarget(RenderTarget& renderTarget){
	}

	void D3DRenderDevice::setRenderTarget(RenderTarget& renderTarget){
		DX11RenderTarget& dxRenderTarget = static_cast<DX11RenderTarget&>(renderTarget);
		currentRenderTarget = dxRenderTarget.getRenderTargetView();
		currentDepthStencil = dxRenderTarget.getDepthStencilView();
		setRenderTargetToCurrent();
	}

	void D3DRenderDevice::resetRenderTargetToDefault(){
		currentRenderTarget = defaultRenderTarget;
		currentDepthStencil = defaultDepthStencil;
		setRenderTargetToCurrent();
	}

	void D3DRenderDevice::removeCurrentGeometryShader(){
		d3dContext->GSSetShader(nullptr, nullptr, 0u);
	}

	void D3DRenderDevice::removeTextureAtSlot(uint32 slot){
		ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
		ID3D11SamplerState* nullST[1] = { nullptr };
		d3dContext->PSSetShaderResources(slot, 1u, nullSRV);
		d3dContext->PSSetSamplers(slot, 1u, nullST);
	}

#if CLV_DEBUG
	DXGIInfoManager& DX11RenderAPI::getInfoManager(){
		return infoManager;
	}
#endif

	void D3DRenderDevice::setRenderTargetToCurrent(){
		//TODO: should maybe use a null depth stencil?
		auto dsv = currentDepthStencil ? currentDepthStencil : defaultDepthStencil;
		if(currentRenderTarget){
			d3dContext->OMSetRenderTargets(1u, currentRenderTarget.GetAddressOf(), dsv.Get());
		} else{
			ID3D11RenderTargetView* nullRTV[1] = { nullptr };
			d3dContext->OMSetRenderTargets(1u, nullRTV, dsv.Get());
		}
	}
}