#include "Clove/Graphics/Direct3D/D3D.hpp"

#include "Clove/Graphics/Direct3D/D3DException.hpp"
#include "Clove/Graphics/Direct3D/D3DRenderFactory.hpp"

#include <d3d11.h>

namespace clv::gfx::d3d{
	std::unique_ptr<RenderFactory> initialiseD3D(){
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

		return std::make_unique<D3DRenderFactory>(d3dDevice, d3dContext);
	}
}