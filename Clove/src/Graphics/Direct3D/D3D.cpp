#include "Clove/Graphics/Direct3D/D3D.hpp"

#include "Clove/Graphics/Direct3D/D3DException.hpp"
#include "Clove/Graphics/Direct3D/D3DRenderDevice.hpp"
#include "Clove/Graphics/Direct3D/D3DRenderFactory.hpp"

#include <d3d11.h>

namespace clv::gfx::d3d{
	std::pair<std::unique_ptr<RenderDevice>, std::unique_ptr<RenderFactory>> initialiseD3D(){
		DX11_INFO_PROVIDER;

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext;
		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice;

		UINT flags = 0;
	#if CLV_DEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0
		};

		DX11_THROW_INFO(D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			flags,
			featureLevels,
			static_cast<UINT>(sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL)),
			D3D11_SDK_VERSION,
			&d3dDevice,
			nullptr,
			&d3dContext
		));

		//TODO: The below should be handled by either application or client
		D3D11_DEPTH_STENCIL_DESC depthDesc{};
		depthDesc.DepthEnable		= TRUE;
		depthDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
		depthDesc.DepthFunc			= D3D11_COMPARISON_LESS;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsstate;
		DX11_THROW_INFO(d3dDevice->CreateDepthStencilState(&depthDesc, &dsstate));
		d3dContext->OMSetDepthStencilState(dsstate.Get(), 1u);

		D3D11_BLEND_DESC blendDesc{};
		blendDesc.AlphaToCoverageEnable					= FALSE;
		blendDesc.IndependentBlendEnable				= FALSE;
		blendDesc.RenderTarget[0].BlendEnable			= TRUE;
		blendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
		DX11_THROW_INFO(d3dDevice->CreateBlendState(&blendDesc, &blendState));
		d3dContext->OMSetBlendState(blendState.Get(), nullptr, 0xffffffff);

		D3D11_RASTERIZER_DESC rdesc{};
		rdesc.FillMode				= D3D11_FILL_SOLID;
		rdesc.CullMode				= D3D11_CULL_BACK;
		rdesc.FrontCounterClockwise = TRUE; //We need to set the front face to CCW to be compatable with opengl/glm
		rdesc.DepthClipEnable		= TRUE;

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rstate;
		DX11_THROW_INFO(d3dDevice->CreateRasterizerState(&rdesc, &rstate));
		d3dContext->RSSetState(rstate.Get());

		d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		CLV_LOG_INFO("Successfully initialised Direct3D");

		auto device = std::make_unique<D3DRenderDevice>(d3dContext);
		auto factory = std::make_unique<D3DRenderFactory>(d3dDevice);

		return std::make_pair(std::move(device), std::move(factory));
	}
}