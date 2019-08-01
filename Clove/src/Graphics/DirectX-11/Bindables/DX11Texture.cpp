#include "clvpch.hpp"
#include "DX11Texture.hpp"

#include "Clove/Application.hpp"
#include "Graphics/DirectX-11/DX11Exception.hpp"
#include "Graphics/DirectX-11/DX11RenderAPI.hpp"

#include <d3d11.h>
#include <stb_image.h>

namespace clv::gfx{
	DX11Texture::DX11Texture(DX11Texture&& other) noexcept = default;

	DX11Texture& DX11Texture::operator=(DX11Texture&& other) noexcept = default;

	DX11Texture::~DX11Texture() = default;

	DX11Texture::DX11Texture(const std::string& filePath, unsigned int bindingPoint)
		: bindingPoint(bindingPoint){
		
		stbi_set_flip_vertically_on_load(1); //DirectX expects our texture to start on the bottom left
		unsigned char* localBuffer = stbi_load(filePath.c_str(), &width, &height, &BPP, 4); //4 = RGBA

		createTexture(usage, localBuffer);

		if(localBuffer){
			stbi_image_free(localBuffer);
		}
	}

	DX11Texture::DX11Texture(int width, int height, TextureUsage usageType, unsigned int bindingPoint)
		: width(width)
		, height(height)
		, usage(usageType)
		, bindingPoint(bindingPoint){

		createTexture(usage, nullptr);
	}

	void DX11Texture::bind(){
		DX11RenderAPI::getContext().PSSetShaderResources(bindingPoint, 1u, textureView.GetAddressOf());
		DX11RenderAPI::getContext().PSSetSamplers(bindingPoint, 1u, sampler.GetAddressOf());
	}

	int DX11Texture::getWidth() const{
		return width;
	}

	int DX11Texture::getHeight() const{
		return height;
	}

	TextureUsage DX11Texture::getUsageType() const{
		return usage;
	}

	const Microsoft::WRL::ComPtr<ID3D11Texture2D>& DX11Texture::getTexture() const{
		return texture;
	}

	void DX11Texture::createTexture(TextureUsage usageType, void* pixels){
		UINT textureBindFlags = D3D11_BIND_SHADER_RESOURCE;
		if(usageType == TextureUsage::RenderTarget){
			textureBindFlags |= D3D11_BIND_RENDER_TARGET;
		}

		//Create the texture itself
		D3D11_TEXTURE2D_DESC textureDesc = { };
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1; //Creating a single texture
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = textureBindFlags;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		
		DX11_INFO_PROVIDER;

		if(pixels){
			D3D11_SUBRESOURCE_DATA data = { };
			data.pSysMem = pixels;
			data.SysMemPitch = width * BPP;

			DX11_THROW_INFO(DX11RenderAPI::getDevice().CreateTexture2D(&textureDesc, &data, &texture));
		} else{
			DX11_THROW_INFO(DX11RenderAPI::getDevice().CreateTexture2D(&textureDesc, nullptr, &texture));
		}

		//Create the view on the texture (what we bind to the pipeline)
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = { };
		viewDesc.Format = textureDesc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MostDetailedMip = 0;
		viewDesc.Texture2D.MipLevels = 1;

		DX11_THROW_INFO(DX11RenderAPI::getDevice().CreateShaderResourceView(texture.Get(), &viewDesc, &textureView));

		//TODO: consider putting this in it's own bindable so it can be reused for multiple textures
		//(will requrie a bit of work though because I'd need to make an interface for OpenGL)

		D3D11_SAMPLER_DESC samplerDesc = { };
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; //min - minification, mag - magnification, min - mip levels.. can also set like anisotrpic here too!
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		DX11_THROW_INFO(DX11RenderAPI::getDevice().CreateSamplerState(&samplerDesc, &sampler));
	}
}