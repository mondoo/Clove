#pragma once

#include "Clove/Graphics/Context.hpp"

#include "Graphics/DirectX-11/DXGIInfoManager.hpp"

#include <wrl.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;

namespace clv::gfx{
	class DXContext : public Context{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext; 
		
		Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;

		//TEMP: just smashing it in here
	#if CLV_DEBUG
		DXGIInfoManager infoManager; //Used by certain DX11 exception macros
	#endif

		//FUNCTIONS
	public:
		DXContext() = delete;
		DXContext(const DXContext& other) = delete;
		DXContext(DXContext&& other) noexcept;
		DXContext& operator=(const DXContext& other) = delete;
		DXContext& operator=(DXContext&& other) noexcept;
		virtual ~DXContext();

		DXContext(void* windowData);

		Microsoft::WRL::ComPtr<ID3D11Device> getDevice() const;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> getContext() const;
		Microsoft::WRL::ComPtr<IDXGISwapChain> getSwapChain() const;

		virtual void present() override;
	};
}
