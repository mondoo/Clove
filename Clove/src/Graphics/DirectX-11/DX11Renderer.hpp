#pragma once

#include "Clove/Graphics/Renderer.hpp"

#include "Graphics/DirectX-11/DXGIInfoManager.hpp"

#include <wrl.h>

struct ID3D11Device;
struct IDXGISwapChain;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace clv::gfx{
	class DX11Renderer : public Renderer{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice; //Device creates
		Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dDeviceContext; //Context issues commands
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv;

	#if CLV_DEBUG
		DXGIInfoManager infoManager; //Used by certain DX11 exception macros
	#endif

		//FUNCTIONS
	public:
		DX11Renderer() = delete;
		DX11Renderer(const DX11Renderer& other) = delete;
		DX11Renderer(DX11Renderer&& other) noexcept = delete;
		DX11Renderer& operator=(const DX11Renderer& other) = delete;
		DX11Renderer& operator=(DX11Renderer&& other) noexcept = delete;
		~DX11Renderer();

		DX11Renderer(const Window& window);

		virtual void clear() override;

		virtual API getAPI() const override;

		inline ID3D11Device& getDevice() const;
		inline ID3D11DeviceContext& getContext() const;

	#if CLV_DEBUG
		inline DXGIInfoManager& getInfoManager();
	#endif

	protected:
		virtual void drawIndexed(const unsigned int count) override;

		virtual void swapBuffers() override;
	};
}

#include "DX11Renderer.inl"