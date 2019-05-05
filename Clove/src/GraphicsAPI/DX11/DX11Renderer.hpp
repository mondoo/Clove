#pragma once

#include "Clove/Graphics/Renderer.hpp"

#include "GraphicsAPI/DX11/DXGIInfoManager.hpp"

#include <wrl.h>

struct ID3D11Device;
struct IDXGISwapChain;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

namespace clv{
	namespace graphics{
		class DX11Renderer : public Renderer{
			//VARIABLES
		private:
			HWND windowsHandle = nullptr;

			Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice; //Device creates
			Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dDeviceContext; //Context issues commands
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target;

		#if CLV_DEBUG
			DXGIInfoManager infoManager; //Used by certain DX11 exception macros
		#endif

			//FUNCTIONS
		public:
			DX11Renderer() = delete;
			DX11Renderer(const DX11Renderer& other) = delete;
			DX11Renderer(DX11Renderer&& other) noexcept = delete;

			DX11Renderer(const Window& window);

			~DX11Renderer();

			virtual void clear() override;
			virtual void drawScene(std::shared_ptr<scene::Scene> scene) override;

			void drawTestTriangle();

			DX11Renderer& operator=(const DX11Renderer& other) = delete;
			DX11Renderer& operator=(DX11Renderer&& other) noexcept = delete;
		};
	}
}

