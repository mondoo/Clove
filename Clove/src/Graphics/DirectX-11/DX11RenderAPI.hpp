#pragma once

#include "Clove/Graphics/RenderAPI.hpp"

#include "Graphics/DirectX-11/DXGIInfoManager.hpp"

#include <wrl.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace clv::gfx{
	class Context;

	class DX11RenderAPI : public RenderAPI{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice; //Device creates
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext; //Context issues commands
		
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv;

	#if CLV_DEBUG
		DXGIInfoManager infoManager; //Used by certain DX11 exception macros
	#endif

		math::Vector4f clearColour = { 0.0f, 0.0f, 0.0f, 0.0f };

		//FUNCTIONS
	public:
		DX11RenderAPI() = delete;
		DX11RenderAPI(const DX11RenderAPI& other) = delete;
		DX11RenderAPI(DX11RenderAPI&& other) noexcept = delete;
		DX11RenderAPI& operator=(const DX11RenderAPI& other) = delete;
		DX11RenderAPI& operator=(DX11RenderAPI&& other) noexcept = delete;
		~DX11RenderAPI();

		DX11RenderAPI(const Context& context);

		virtual void clear() override;
		virtual void drawIndexed(const unsigned int count) override;

		virtual void setClearColour(const math::Vector4f& colour) override;

		//TODO: Is there anyway to get rid of these?
		ID3D11Device& getDevice() const;
		ID3D11DeviceContext& getContext() const;
		//...

	#if CLV_DEBUG
		DXGIInfoManager& getInfoManager();
	#endif
	};
}