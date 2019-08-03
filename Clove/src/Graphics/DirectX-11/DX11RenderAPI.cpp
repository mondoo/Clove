#include "clvpch.hpp"
#include "DX11RenderAPI.hpp"

#include "Graphics/DirectX-11/DXContext.hpp"
#include "Graphics/DirectX-11/DX11Exception.hpp"
#include "Graphics/DirectX-11/DX11RenderTarget.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>

namespace clv::gfx{
	Microsoft::WRL::ComPtr<ID3D11Device> DX11RenderAPI::d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> DX11RenderAPI::d3dContext;

#if CLV_DEBUG
	DXGIInfoManager DX11RenderAPI::infoManager;
#endif

	DX11RenderAPI::~DX11RenderAPI() = default;

	DX11RenderAPI::DX11RenderAPI(const Context& context)
		: RenderAPI(context.getAPI()){
		if(const DXContext * dxCon = dynamic_cast<const DXContext*>(&context)){
			d3dDevice = dxCon->getDevice();
			d3dContext = dxCon->getContext();
			defaultRenderTarget = dxCon->getTarget();
			currentRenderTarget = defaultRenderTarget;
			dsv = dxCon->getDSV();

			DX11_INFO_PROVIDER;

			//Setting up the default depth stencil
			D3D11_DEPTH_STENCIL_DESC depthDesc = {};
			depthDesc.DepthEnable = TRUE;
			depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			depthDesc.DepthFunc = D3D11_COMPARISON_LESS;

			Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsstate;
			DX11_THROW_INFO(d3dDevice->CreateDepthStencilState(&depthDesc, &dsstate));

			d3dContext->OMSetDepthStencilState(dsstate.Get(), 1u);

			//Setting up the default blend state
			D3D11_BLEND_DESC blendDesc = { };
			blendDesc.AlphaToCoverageEnable = FALSE;
			blendDesc.IndependentBlendEnable = FALSE;
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
			DX11_THROW_INFO(d3dDevice->CreateBlendState(&blendDesc, &blendState));

			d3dContext->OMSetBlendState(blendState.Get(), nullptr, 0xffffffff);

			//Set primitive topology to triangle list (groups of 3 verticies)
			d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//Setting up the default resterizer state
			D3D11_RASTERIZER_DESC rdesc = {};
			rdesc.FillMode = D3D11_FILL_SOLID;
			rdesc.CullMode = D3D11_CULL_BACK;
			rdesc.FrontCounterClockwise = TRUE; //We need to set the front face to CCW to be compatable with opengl/glm
			rdesc.DepthClipEnable = TRUE;

			Microsoft::WRL::ComPtr<ID3D11RasterizerState> rstate;
			DX11_THROW_INFO(d3dDevice->CreateRasterizerState(&rdesc, &rstate));

			d3dContext->RSSetState(rstate.Get());

			CLV_LOG_DEBUG("Created DX11 renderer");
		} else{
			CLV_LOG_ERROR("Couldn't retrieve data from context, unable to create DX11 renderer properly");
		}
	}

	void DX11RenderAPI::clear(){
		d3dContext->ClearRenderTargetView(currentRenderTarget.Get(), math::valuePtr(clearColour));
		d3dContext->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0xff);
	}

	void DX11RenderAPI::drawIndexed(const unsigned int count){
		DX11_THROW_INFO_ONLY(d3dContext->DrawIndexed(static_cast<UINT>(count), 0u, 0u));
	}

	void DX11RenderAPI::setClearColour(const math::Vector4f& colour){
		clearColour = colour;
	}

	void DX11RenderAPI::setDepthBuffer(bool enabled){
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

	void DX11RenderAPI::setBlendState(bool enabled){
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

	void DX11RenderAPI::setRenderTarget(RenderTarget& renderTarget){
		DX11RenderTarget& dxRenderTarget = static_cast<DX11RenderTarget&>(renderTarget);
		currentRenderTarget = dxRenderTarget.getRenderTargetView();
		setRenderTargetToCurrent();
	}

	void DX11RenderAPI::resetRenderTarget(){
		currentRenderTarget = defaultRenderTarget;
		setRenderTargetToCurrent();
	}

	ID3D11Device& DX11RenderAPI::getDevice(){
		CLV_ASSERT(d3dDevice != nullptr, __FUNCTION__" called with null device");
		return *d3dDevice.Get();
	}

	ID3D11DeviceContext& DX11RenderAPI::getContext(){
		CLV_ASSERT(d3dContext != nullptr, __FUNCTION__" called with null context");
		return *d3dContext.Get();
	}

#if CLV_DEBUG
	DXGIInfoManager& DX11RenderAPI::getInfoManager(){
		return infoManager;
	}
#endif

	void DX11RenderAPI::setRenderTargetToCurrent(){
		d3dContext->OMSetRenderTargets(1u, currentRenderTarget.GetAddressOf(), dsv.Get());
	}
}