#include "clvpch.hpp"
#include "DX11Texture.hpp"

#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Graphics/DirectX-11/DX11Exception.hpp"
#include "Graphics/DirectX-11/DX11Renderer.hpp"

#include <d3d11.h>
#include <stb_image.h>

namespace clv::gfx{
	DX11Texture::DX11Texture(DX11Texture&& other) noexcept = default;

	DX11Texture& DX11Texture::operator=(DX11Texture&& other) noexcept = default;

	DX11Texture::~DX11Texture() = default;

	DX11Texture::DX11Texture(const std::string& filePath){
		Renderer& renderer = Application::get().getWindow().getRenderer();
		if(DX11Renderer* dxrenderer = dynamic_cast<DX11Renderer*>(&renderer)){
			DX11_INFO_PROVIDER(dxrenderer);

			unsigned char* localBuffer = stbi_load(filePath.c_str(), &width, &height, &BPP, 4); //4 = RGBA

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
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA data = { };
			data.pSysMem = localBuffer;
			data.SysMemPitch = width * BPP;

			Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;

			DX11_THROW_INFO(dxrenderer->getDevice().CreateTexture2D(&textureDesc, &data, &texture));

			if(localBuffer){
				stbi_image_free(localBuffer);
			}

			//Create the view on the texture (what we bind to the pipeline)
			D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = { };
			viewDesc.Format = textureDesc.Format;
			viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			viewDesc.Texture2D.MostDetailedMip = 0;
			viewDesc.Texture2D.MipLevels = 1;
			
			DX11_THROW_INFO(dxrenderer->getDevice().CreateShaderResourceView(texture.Get(), &viewDesc, &textureView));
		
			//TODO: consider putting this in it's own bindable so it can be reused for multiple textures
			//(will requrie a bit of work though because I'd need to make an interface for OpenGL)

			D3D11_SAMPLER_DESC samplerDesc = { };
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; //min - minification, mag - magnification, min - mip levels.. can also set like anisotrpic here too!
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

			DX11_THROW_INFO(dxrenderer->getDevice().CreateSamplerState(&samplerDesc, &sampler));
		}
	}

	void DX11Texture::bind(Renderer& renderer){
		if(DX11Renderer* dxrenderer = dynamic_cast<DX11Renderer*>(&renderer)){
			//First param is register being access on shader side
			dxrenderer->getContext().PSSetShaderResources(0u, 1u, textureView.GetAddressOf());
			dxrenderer->getContext().PSSetSamplers(0u, 1u, sampler.GetAddressOf());
		}
	}

	void DX11Texture::unbind(){
	}

	int DX11Texture::getWidth() const{
		return width;
	}

	int DX11Texture::getHeight() const{
		return height;
	}
}