#pragma once

#include "Clove/Graphics/Context.hpp"

#include "Clove/Graphics/DirectX-11/DXGIInfoManager.hpp"

#include <wrl.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace clv::gfx{
	class DXContext : public Context{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext; 
		
		Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv;

		//TEMP: just smashing it in here
	#if CLV_DEBUG
		DXGIInfoManager infoManager; //Used by certain DX11 exception macros
	#endif

		unsigned int swapInterval = 1u;

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
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> getTarget() const;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> getDSV() const;

		virtual void makeCurrent() override;

		virtual void setVSync(bool enabled) override;
		virtual bool isVsync() const override;

		virtual API getAPI() const override;

		virtual void present() override;
	};
}
