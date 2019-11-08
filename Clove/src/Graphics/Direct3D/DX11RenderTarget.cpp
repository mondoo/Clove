#include "DX11RenderTarget.hpp"

#include "Graphics/DirectX-11/Bindables/DX11Texture.hpp"
#include "Graphics/DirectX-11/DX11Exception.hpp"
#include "Graphics/DirectX-11/DX11RenderAPI.hpp"

#include <d3d11.h>

namespace clv::gfx{
	DX11RenderTarget::DX11RenderTarget(DX11RenderTarget&& other) noexcept = default;

	DX11RenderTarget& DX11RenderTarget::operator=(DX11RenderTarget&& other) noexcept = default;

	DX11RenderTarget::~DX11RenderTarget() = default;

	DX11RenderTarget::DX11RenderTarget(Texture* colourTexture, Texture* depthStencilTexture){
		CLV_ASSERT(colourTexture != nullptr || depthStencilTexture != nullptr, "{0}: Render target needs at least one valid texture", CLV_FUNCTION_NAME);

		DX11_INFO_PROVIDER;

		//Colour view
		if(colourTexture){
			DX11Texture* dxTexture = static_cast<DX11Texture*>(colourTexture);
			Microsoft::WRL::ComPtr<ID3D11Texture2D> textureSource = dxTexture->getTexture();

			D3D11_TEXTURE2D_DESC textureDesc;
			textureSource->GetDesc(&textureDesc);

			D3D11_RENDER_TARGET_VIEW_DESC rtvdsc = { };
			rtvdsc.Format = textureDesc.Format;
			rtvdsc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvdsc.Texture2D.MipSlice = 0;

			DX11_THROW_INFO(DX11RenderAPI::getDevice().CreateRenderTargetView(textureSource.Get(), &rtvdsc, &renderTargetView));
		}

		//Depth / Stencil view
		if(depthStencilTexture){
			DX11Texture* dxTexture = static_cast<DX11Texture*>(depthStencilTexture);
			Microsoft::WRL::ComPtr<ID3D11Texture2D> textureSource = dxTexture->getTexture();

			D3D11_TEXTURE2D_DESC textureDesc = {};
			textureSource->GetDesc(&textureDesc);

			const D3D11_DSV_DIMENSION dimension = dxTexture->getTextureStyle() == TextureStyle::Cubemap ? D3D11_DSV_DIMENSION_TEXTURE2DARRAY : D3D11_DSV_DIMENSION_TEXTURE2D;

			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
			dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; //NOTE: not supporting stencil at the moment
			dsvDesc.ViewDimension = dimension;
			if(dxTexture->getTextureStyle() == TextureStyle::Cubemap){
				dsvDesc.Texture2DArray.ArraySize = textureDesc.ArraySize;
				dsvDesc.Texture2DArray.FirstArraySlice = 0u;
				dsvDesc.Texture2DArray.MipSlice = 0u;
			} else{
				dsvDesc.Texture2D.MipSlice = 0u;
			}

			DX11_THROW_INFO(DX11RenderAPI::getDevice().CreateDepthStencilView(textureSource.Get(), &dsvDesc, &depthStencilView));
		}
	}

	DX11RenderTarget::DX11RenderTarget(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView, Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView)
		: renderTargetView(renderTargetView)
		, depthStencilView(depthStencilView){
	}

	const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& DX11RenderTarget::getRenderTargetView() const{
		return renderTargetView;
	}

	const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& DX11RenderTarget::getDepthStencilView() const{
		return depthStencilView;
	}
}