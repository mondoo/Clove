#pragma once

#include "Clove/Graphics/RenderTarget.hpp"

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
		std::shared_ptr<GraphicsFactory> factory;

		mth::vec4f clearColour{ 0.0f, 0.0f, 0.0f, 1.0f };

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

		//FUNCTIONS
	public:
		D3DRenderTarget() = delete;
		D3DRenderTarget(std::shared_ptr<GraphicsFactory> factory, ID3D11Device& d3dDevice, Texture* colourTexture, Texture* depthStencilTexture);
		D3DRenderTarget(std::shared_ptr<GraphicsFactory> factory, Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView, Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView);
		
		D3DRenderTarget(const D3DRenderTarget& other) = delete;
		D3DRenderTarget(D3DRenderTarget&& other) noexcept;
		
		D3DRenderTarget& operator=(const D3DRenderTarget& other) = delete;
		D3DRenderTarget& operator=(D3DRenderTarget&& other) noexcept;
		
		~D3DRenderTarget();

		const std::shared_ptr<GraphicsFactory>& getFactory() const override;

		void clear() override;

		void setClearColour(const mth::vec4f& colour) override;
		const mth::vec4f& getClearColour() const;

		void clearTextureViews();
		void updateTextureViews(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView, Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView);

		const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& getRenderTargetView() const;
		const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& getDepthStencilView() const;
	};
}
