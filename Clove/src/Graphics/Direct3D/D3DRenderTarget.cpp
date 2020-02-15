#include "Clove/Graphics/Direct3D/D3DRenderTarget.hpp"

#include "Clove/Graphics/Direct3D/Resources/D3DTexture.hpp"
#include "Clove/Graphics/Direct3D/D3DException.hpp"

#include <d3d11.h>

namespace clv::gfx::d3d{
	D3DRenderTarget::D3DRenderTarget(ID3D11Device& d3dDevice, Texture* colourTexture, Texture* depthStencilTexture){
		CLV_ASSERT(colourTexture != nullptr || depthStencilTexture != nullptr, "{0}: Render target needs at least one valid texture", CLV_FUNCTION_NAME);

		DX11_INFO_PROVIDER;

		//Colour view
		if(colourTexture){
			D3DTexture* d3dTexture = static_cast<D3DTexture*>(colourTexture);
			Microsoft::WRL::ComPtr<ID3D11Texture2D> textureSource = d3dTexture->getD3DTexture();

			D3D11_TEXTURE2D_DESC textureDesc;
			textureSource->GetDesc(&textureDesc);

			D3D11_RENDER_TARGET_VIEW_DESC rtvdsc{};
			rtvdsc.Format				= textureDesc.Format;
			rtvdsc.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvdsc.Texture2D.MipSlice	= 0;

			DX11_THROW_INFO(d3dDevice.CreateRenderTargetView(textureSource.Get(), &rtvdsc, &renderTargetView));
		}

		//Depth / Stencil view
		if(depthStencilTexture){
			D3DTexture* d3dTexture = static_cast<D3DTexture*>(depthStencilTexture);
			Microsoft::WRL::ComPtr<ID3D11Texture2D> textureSource = d3dTexture->getD3DTexture();

			D3D11_TEXTURE2D_DESC textureDesc{};
			textureSource->GetDesc(&textureDesc);

			const TextureStyle textureStyle = d3dTexture->getDescriptor().style;
			const D3D11_DSV_DIMENSION dimension = textureStyle == TextureStyle::Cubemap ? D3D11_DSV_DIMENSION_TEXTURE2DARRAY : D3D11_DSV_DIMENSION_TEXTURE2D;

			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
			dsvDesc.Format								= DXGI_FORMAT_D32_FLOAT; //NOTE: not supporting stencil at the moment
			dsvDesc.ViewDimension						= dimension;
			if(textureStyle == TextureStyle::Cubemap){
				dsvDesc.Texture2DArray.ArraySize		= textureDesc.ArraySize;
				dsvDesc.Texture2DArray.FirstArraySlice	= 0u;
				dsvDesc.Texture2DArray.MipSlice			= 0u;
			} else{
				dsvDesc.Texture2D.MipSlice				= 0u;
			}

			DX11_THROW_INFO(d3dDevice.CreateDepthStencilView(textureSource.Get(), &dsvDesc, &depthStencilView));
		}

		setClearColour({ 0.0f, 0.0f, 0.0f, 0.0f });
	}

	D3DRenderTarget::D3DRenderTarget(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView, Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView)
		: renderTargetView(renderTargetView)
		, depthStencilView(depthStencilView){
	}

	D3DRenderTarget::D3DRenderTarget(D3DRenderTarget&& other) noexcept = default;

	D3DRenderTarget& D3DRenderTarget::operator=(D3DRenderTarget && other) noexcept = default;

	D3DRenderTarget::~D3DRenderTarget() = default;

	void D3DRenderTarget::clear(){
		Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
		if (renderTargetView){
			renderTargetView->GetDevice(&device);
		} else if (depthStencilView){
			depthStencilView->GetDevice(&device);
		} else{
			CLV_LOG_ERROR("{0}: could not retrieve device", CLV_FUNCTION_NAME_PRETTY);
			return;
		}

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = nullptr;
		device->GetImmediateContext(&context);

		if (renderTargetView){
			context->ClearRenderTargetView(renderTargetView.Get(), mth::valuePtr(clearColour));
		}
		if (depthStencilView){
			context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0xff);
		}
	}

	void D3DRenderTarget::setClearColour(const mth::vec4f& colour){
		clearColour = colour;
	}

	const mth::vec4f& D3DRenderTarget::getClearColour() const{
		return clearColour;
	}

	const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& D3DRenderTarget::getRenderTargetView() const{
		return renderTargetView;
	}

	const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& D3DRenderTarget::getDepthStencilView() const{
		return depthStencilView;
	}
}