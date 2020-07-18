#include "Clove/Graphics/Direct3D/D3D.hpp"

#include "Clove/Graphics/Direct3D/D3DException.hpp"
#include "Clove/Graphics/Direct3D/D3DGraphicsFactory.hpp"

#include <d3d11.h>

namespace clv::gfx::d3d{
	std::shared_ptr<GraphicsFactory> initialiseD3D() {
		DX11_INFO_PROVIDER;

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext;
		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice;

		UINT flags = 0;
	#if GARLIC_DEBUG
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

		GARLIC_LOG(garlicLogContext, Log::Level::Info, "Successfully initialised Direct3D");

		return std::make_shared<D3DGraphicsFactory>(d3dDevice, d3dContext);
	}
}