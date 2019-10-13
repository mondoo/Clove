#pragma once

#include "Clove/Graphics/RenderTarget.hpp"

#include <wrl.h>

struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace clv::gfx{
	class DX11RenderTarget : public RenderTarget{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

		//FUNCTIONS
	public:
		DX11RenderTarget() = delete;
		DX11RenderTarget(const DX11RenderTarget& other) = delete;
		DX11RenderTarget(DX11RenderTarget&& other) noexcept;
		DX11RenderTarget& operator=(const DX11RenderTarget& other) = delete;
		DX11RenderTarget& operator=(DX11RenderTarget&& other) noexcept;
		virtual ~DX11RenderTarget();
		
		DX11RenderTarget(Texture* colourTexture, Texture* depthStencilTexture);

		const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& getRenderTargetView() const;
		const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& getDepthStencilView() const;
	};
}
