#include "DX11Texture.hpp"

#include "Core/Application.hpp"
#include "Graphics/DirectX-11/DX11Exception.hpp"
#include "Graphics/DirectX-11/DX11RenderAPI.hpp"

#include <stb_image.h>

namespace clv::gfx{
	DX11Texture::DX11Texture(DX11Texture&& other) noexcept = default;

	DX11Texture& DX11Texture::operator=(DX11Texture&& other) noexcept = default;

	DX11Texture::~DX11Texture() = default;

	DX11Texture::DX11Texture(const std::string& filePath, uint32 bindingPoint)
		: filePath(filePath), bindingPoint(bindingPoint){
		stbi_set_flip_vertically_on_load(1); //DirectX expects our texture to start on the bottom left
		
		int width = 0;
		int height = 0;
		unsigned char* localBuffer = stbi_load(filePath.c_str(), &width, &height, &BPP, 4); //4 = RGBA

		descriptor.dimensions.x = static_cast<uint32>(width);
		descriptor.dimensions.y = static_cast<uint32>(height);

		createTexture(localBuffer);

		if(localBuffer){
			stbi_image_free(localBuffer);
		}
	}

	DX11Texture::DX11Texture(void* bufferData, uint32 bindingPoint, const gfx::TextureDescriptor& descriptor)
		: bindingPoint(bindingPoint), descriptor(descriptor){
		createTexture(bufferData);
	}

	DX11Texture::DX11Texture(uint32 bindingPoint, const gfx::TextureDescriptor& descriptor)
		: bindingPoint(bindingPoint), descriptor(descriptor){
		createTexture(nullptr);
	}

	void DX11Texture::bind(){
		DX11RenderAPI::getContext().PSSetShaderResources(bindingPoint, 1u, textureView.GetAddressOf());
		DX11RenderAPI::getContext().PSSetSamplers(bindingPoint, 1u, sampler.GetAddressOf());
	}

	uint32 DX11Texture::getWidth() const{
		return descriptor.dimensions.x;
	}

	uint32 DX11Texture::getHeight() const{
		return descriptor.dimensions.x;
	}

	TextureBindingPoint DX11Texture::getBindingPoint() const{
		return static_cast<TextureBindingPoint>(bindingPoint);
	}

	TextureUsage DX11Texture::getUsageType() const{
		return descriptor.usage;
	}

	TextureStyle DX11Texture::getTextureStyle() const{
		return descriptor.style;
	}

	const Microsoft::WRL::ComPtr<ID3D11Texture2D>& DX11Texture::getTexture() const{
		return texture;
	}

	void DX11Texture::createTexture(void* pixels){
		DX11_INFO_PROVIDER;
		
		//If this is being used as a font then the BPP needs to be 1
		if(descriptor.usage == TextureUsage::Font){
			BPP = 1;
		}

		const auto textureDescriptor = createD3DTextureDescriptor();

		if(pixels){
			CLV_ASSERT(descriptor.arraySize == 1, "DX Textures with array size greater than 1 not supported");

			D3D11_SUBRESOURCE_DATA data = { };
			data.pSysMem = pixels;
			data.SysMemPitch = descriptor.dimensions.x * BPP;

			DX11_THROW_INFO(DX11RenderAPI::getDevice().CreateTexture2D(&textureDescriptor, &data, &texture));
		} else{
			DX11_THROW_INFO(DX11RenderAPI::getDevice().CreateTexture2D(&textureDescriptor, nullptr, &texture));
		}

		const auto viewDescriptor = createD3DShaderViewDescriptor();
		const auto smaplerDescriptor = createD3DSamplerDescriptor();

		DX11_THROW_INFO(DX11RenderAPI::getDevice().CreateShaderResourceView(texture.Get(), &viewDescriptor, &textureView));
		DX11_THROW_INFO(DX11RenderAPI::getDevice().CreateSamplerState(&smaplerDescriptor, &sampler));
	}

	D3D11_TEXTURE2D_DESC DX11Texture::createD3DTextureDescriptor() const{
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = static_cast<UINT>(descriptor.dimensions.x);
		textureDesc.Height = static_cast<UINT>(descriptor.dimensions.y);
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = getArrayElements(descriptor.style) * descriptor.arraySize;
		textureDesc.Format = getFormatForTexture(descriptor.usage);
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = getBindFlags(descriptor.usage);
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = getMiscFlags(descriptor.style);

		return textureDesc;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC DX11Texture::createD3DShaderViewDescriptor() const{
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
		viewDesc.Format = getFormatForShaderView(descriptor.usage);
		viewDesc.ViewDimension = getViewDimension(descriptor.style, descriptor.arraySize);
		
		switch(viewDesc.ViewDimension){
			case D3D11_SRV_DIMENSION_TEXTURE2D:
				viewDesc.Texture2D.MostDetailedMip = 0u;
				viewDesc.Texture2D.MipLevels = 1u;
				break;

			case D3D11_SRV_DIMENSION_TEXTURE2DARRAY:
				viewDesc.Texture2DArray.MostDetailedMip = 0u;
				viewDesc.Texture2DArray.MipLevels = 1u;
				viewDesc.Texture2DArray.FirstArraySlice = 0u;
				viewDesc.Texture2DArray.ArraySize = descriptor.arraySize;
				break;

			case D3D11_SRV_DIMENSION_TEXTURECUBE:
				viewDesc.TextureCube.MostDetailedMip = 0u;
				viewDesc.TextureCube.MipLevels = 1u;
				break;

			case D3D11_SRV_DIMENSION_TEXTURECUBEARRAY:
				viewDesc.TextureCubeArray.MostDetailedMip = 0u;
				viewDesc.TextureCubeArray.MipLevels = 1u;
				viewDesc.TextureCubeArray.First2DArrayFace = 0u;
				viewDesc.TextureCubeArray.NumCubes = descriptor.arraySize;
				break;

			default:
				CLV_ASSERT(false, "Unhandled dimension in {0}" CLV_FUNCTION_NAME);
				break;
		}

		return viewDesc;
	}

	D3D11_SAMPLER_DESC DX11Texture::createD3DSamplerDescriptor() const{
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

		return samplerDesc;
	}

	UINT DX11Texture::getBindFlags(const TextureUsage usage) const{
		switch(usage){
			case TextureUsage::Default:
			case TextureUsage::Font:
				return D3D11_BIND_SHADER_RESOURCE;

			case TextureUsage::RenderTarget_Colour:
				return D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

			case TextureUsage::RenderTarget_Depth:
				return D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;

			default:
				CLV_ASSERT(false, "Unkown type in {0}", CLV_FUNCTION_NAME);
				return D3D11_BIND_SHADER_RESOURCE;
		}
	}

	DXGI_FORMAT DX11Texture::getFormatForTexture(const TextureUsage usage) const{
		switch(usage){
			case TextureUsage::Default:
			case TextureUsage::RenderTarget_Colour:
				return DXGI_FORMAT_R8G8B8A8_UNORM;

			case TextureUsage::RenderTarget_Depth:
				return DXGI_FORMAT_R32_TYPELESS;

			case TextureUsage::Font:
				return DXGI_FORMAT_R8_UNORM;

			default:
				CLV_ASSERT(false, "Unkown type in {0}", CLV_FUNCTION_NAME);
				return DXGI_FORMAT_R8G8B8A8_UNORM;
		}
	}

	DXGI_FORMAT DX11Texture::getFormatForShaderView(const TextureUsage usage) const{
		return (usage == TextureUsage::RenderTarget_Depth) ? DXGI_FORMAT_R32_FLOAT : getFormatForTexture(usage);
	}

	UINT DX11Texture::getArrayElements(const TextureStyle style) const{
		return (style == TextureStyle::Cubemap) ? 6 : 1;
	}

	D3D_SRV_DIMENSION DX11Texture::getViewDimension(const TextureStyle style, const uint8 arraySize) const{
		switch(style){
			case TextureStyle::Default:
				return (arraySize > 1) ? D3D11_SRV_DIMENSION_TEXTURE2DARRAY : D3D11_SRV_DIMENSION_TEXTURE2D;

			case TextureStyle::Cubemap:
				return (arraySize > 1) ? D3D11_SRV_DIMENSION_TEXTURECUBEARRAY : D3D11_SRV_DIMENSION_TEXTURECUBE;

			default:
				CLV_ASSERT(false, "Unkown type in {0}", CLV_FUNCTION_NAME);
				return D3D11_SRV_DIMENSION_TEXTURE2D;
		}
	}

	UINT DX11Texture::getMiscFlags(const TextureStyle style) const{
		return (style == TextureStyle::Cubemap) ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;
	}
}