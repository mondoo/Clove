#pragma once

#include "Clove/Graphics/Core/RenderTarget.hpp"

#include <wrl.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace clv::gfx{
	class Texture;
}

namespace clv::gfx::d3d{
	class D3DRenderTarget : public RenderTarget{
		//VARIABLES
	private:
		mth::vec4f clearColour{ 0.0f, 0.0f, 0.0f, 1.0f };

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

		virtual void clear() override;

		virtual void setClearColour(const mth::vec4f& colour) override;
		const mth::vec4f& getClearColour() const;

		const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& getRenderTargetView() const;
		const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& getDepthStencilView() const;
	};
}
