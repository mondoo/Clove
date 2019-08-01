#include "clvpch.hpp"
#include "DX11FrameBuffer.hpp"

#include "Graphics/DirectX-11/Bindables/DX11Texture.hpp"
#include "Graphics/DirectX-11/DX11Exception.hpp"
#include "Graphics/DirectX-11/DX11RenderAPI.hpp"

#include <d3d11.h>

namespace clv::gfx{
	DX11FrameBuffer::DX11FrameBuffer(){
		//TODO:
		//probably need to create the default one here rather than in attach texture
	}

	DX11FrameBuffer::DX11FrameBuffer(DX11FrameBuffer&& other) noexcept = default;

	DX11FrameBuffer& DX11FrameBuffer::operator=(DX11FrameBuffer&& other) noexcept = default;

	DX11FrameBuffer::~DX11FrameBuffer() = default;

	void DX11FrameBuffer::bind(){
		//TODO
	}

	void DX11FrameBuffer::attachTexture(Texture& texture){
		DX11Texture& dxTexture = static_cast<DX11Texture&>(texture);
		Microsoft::WRL::ComPtr<ID3D11Texture2D> textureSource = dxTexture.getTexture();

		D3D11_TEXTURE2D_DESC textureDesc;
		textureSource->GetDesc(&textureDesc);

		D3D11_RENDER_TARGET_VIEW_DESC rtvdsc = { };
		rtvdsc.Format = textureDesc.Format;
		rtvdsc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvdsc.Texture2D.MipSlice = 0;

		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(DX11RenderAPI::getDevice().CreateRenderTargetView(textureSource.Get(), &rtvdsc, &renderTargetView));
	}

	bool DX11FrameBuffer::isComplete() const{
		//TODO
		return false;
	}
}