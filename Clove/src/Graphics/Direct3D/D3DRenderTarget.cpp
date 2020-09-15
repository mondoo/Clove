#include "Clove/Graphics/Direct3D/D3DRenderTarget.hpp"

#include "Clove/Graphics/Direct3D/D3DException.hpp"
#include "Clove/Graphics/Direct3D/D3DTexture.hpp"
#include "Clove/Log.hpp"

#include <d3d11.h>
#include <Root/Definitions.hpp>

namespace clv::gfx::d3d {
	D3DRenderTarget::D3DRenderTarget(std::shared_ptr<GraphicsFactory> factory, ID3D11Device& d3dDevice, Texture* colourTexture, Texture* depthStencilTexture)
		: factory(std::move(factory)) {
		GARLIC_ASSERT(colourTexture != nullptr || depthStencilTexture != nullptr, "{0}: Render target needs at least one valid texture", GARLIC_FUNCTION_NAME);

		DX11_INFO_PROVIDER;

		//Colour view
		if(colourTexture != nullptr) {
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
		if(depthStencilTexture != nullptr) {
			D3DTexture* d3dTexture = static_cast<D3DTexture*>(depthStencilTexture);
			Microsoft::WRL::ComPtr<ID3D11Texture2D> textureSource = d3dTexture->getD3DTexture();

			D3D11_TEXTURE2D_DESC textureDesc{};
			textureSource->GetDesc(&textureDesc);

			const TextureStyle textureStyle = d3dTexture->getDescriptor().style;

			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
			dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; //NOTE: not supporting stencil at the moment
			if(textureStyle == TextureStyle::Cubemap || textureDesc.ArraySize > 1) {
				dsvDesc.ViewDimension					= D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
				dsvDesc.Texture2DArray.ArraySize		= textureDesc.ArraySize;
				dsvDesc.Texture2DArray.FirstArraySlice	= 0u;
				dsvDesc.Texture2DArray.MipSlice			= 0u;
			} else {
				dsvDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
				dsvDesc.Texture2D.MipSlice	= 0u;
			}

			DX11_THROW_INFO(d3dDevice.CreateDepthStencilView(textureSource.Get(), &dsvDesc, &depthStencilView));
		}

		setClearColour({ 0.0f, 0.0f, 0.0f, 0.0f });
	}

	D3DRenderTarget::D3DRenderTarget(std::shared_ptr<GraphicsFactory> factory, Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView, Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView)
		: factory(std::move(factory))
		, renderTargetView(std::move(renderTargetView))
		, depthStencilView(std::move(depthStencilView)) {
	}

	D3DRenderTarget::D3DRenderTarget(D3DRenderTarget&& other) noexcept = default;

	D3DRenderTarget& D3DRenderTarget::operator=(D3DRenderTarget&& other) noexcept = default;

	D3DRenderTarget::~D3DRenderTarget() = default;

	const std::shared_ptr<GraphicsFactory>& D3DRenderTarget::getFactory() const {
		return factory;
	}

	void D3DRenderTarget::clear() {
		Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
		if(renderTargetView) {
			renderTargetView->GetDevice(&device);
		} else if(depthStencilView) {
			depthStencilView->GetDevice(&device);
		} else {
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "{0}: could not retrieve device", GARLIC_FUNCTION_NAME_PRETTY);
			return;
		}

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = nullptr;
		device->GetImmediateContext(&context);

		if(renderTargetView) {
			context->ClearRenderTargetView(renderTargetView.Get(), mth::valuePtr(clearColour));
		}
		if(depthStencilView) {
			context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0xff);
		}
	}

	void D3DRenderTarget::setClearColour(const mth::vec4f& colour) {
		clearColour = colour;
	}

	const mth::vec4f& D3DRenderTarget::getClearColour() const {
		return clearColour;
	}

	void D3DRenderTarget::clearTextureViews() {
		renderTargetView.Reset();
		depthStencilView.Reset();
	}

	void D3DRenderTarget::updateTextureViews(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView, Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView) {
		this->renderTargetView = std::move(renderTargetView);
		this->depthStencilView = std::move(depthStencilView);
	}

	const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& D3DRenderTarget::getRenderTargetView() const {
		return renderTargetView;
	}

	const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& D3DRenderTarget::getDepthStencilView() const {
		return depthStencilView;
	}
}