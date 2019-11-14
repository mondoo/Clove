#pragma once

#include "Core/Graphics/RenderTarget.hpp"

#include <wrl.h>

struct ID3D11Device;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace clv::gfx{
	class Texture;
}

namespace clv::gfx::d3d::_11{
	class D3DRenderTarget : public RenderTarget{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

		//FUNCTIONS
	public:
		D3DRenderTarget() = delete;
		D3DRenderTarget(ID3D11Device& d3dDevice, Texture* colourTexture, Texture* depthStencilTexture);
		D3DRenderTarget(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView, Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView);
		D3DRenderTarget(const D3DRenderTarget& other) = delete;
		D3DRenderTarget(D3DRenderTarget&& other) noexcept;
		D3DRenderTarget& operator=(const D3DRenderTarget& other) = delete;
		D3DRenderTarget& operator=(D3DRenderTarget&& other) noexcept;
		virtual ~D3DRenderTarget();

		const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& getRenderTargetView() const;
		const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& getDepthStencilView() const;
	};
}
