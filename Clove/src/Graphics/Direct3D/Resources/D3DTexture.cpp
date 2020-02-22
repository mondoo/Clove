#include "Clove/Graphics/Direct3D/Resources/D3DTexture.hpp"

#include "Clove/Graphics/Direct3D/D3DException.hpp"

#include <stb_image.h>

namespace clv::gfx::d3d{
	static DXGI_FORMAT getFormatForTexture(const TextureUsage usage){
		switch (usage){
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

	static DXGI_FORMAT getFormatForShaderView(const TextureUsage usage){
		return (usage == TextureUsage::RenderTarget_Depth) ? DXGI_FORMAT_R32_FLOAT : getFormatForTexture(usage);
	}

	static D3D_SRV_DIMENSION getViewDimension(const TextureStyle style, const uint8_t arraySize){
		switch (style){
		case TextureStyle::Default:
			return (arraySize > 1) ? D3D11_SRV_DIMENSION_TEXTURE2DARRAY : D3D11_SRV_DIMENSION_TEXTURE2D;

		case TextureStyle::Cubemap:
			return (arraySize > 1) ? D3D11_SRV_DIMENSION_TEXTURECUBEARRAY : D3D11_SRV_DIMENSION_TEXTURECUBE;

		default:
			CLV_ASSERT(false, "Unkown type in {0}", CLV_FUNCTION_NAME);
			return D3D11_SRV_DIMENSION_TEXTURE2D;
		}
	}

	static D3D11_SHADER_RESOURCE_VIEW_DESC createD3DShaderViewDescriptor(const TextureDescriptor& descriptor){
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
		viewDesc.Format = getFormatForShaderView(descriptor.usage);
		viewDesc.ViewDimension = getViewDimension(descriptor.style, descriptor.arraySize);

		switch (viewDesc.ViewDimension){
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

	static UINT getBindFlags(const TextureUsage usage){
		switch (usage){
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

	static UINT getArrayElements(const TextureStyle style){
		return (style == TextureStyle::Cubemap) ? 6 : 1;
	}

	static UINT getMiscFlags(const TextureStyle style){
		return (style == TextureStyle::Cubemap) ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;
	}

	static D3D11_FILTER getFilter(const TextureFilter filter){
		switch (filter){
		case TextureFilter::Nearest:
			return D3D11_FILTER_MIN_MAG_MIP_POINT;

		case TextureFilter::Linear:
			return D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		default:
			CLV_ASSERT(false, "Unkown type in {0}", CLV_FUNCTION_NAME);
			return D3D11_FILTER_MIN_MAG_MIP_POINT;
		}
	}

	D3DTexture::D3DTexture(ID3D11Device& d3dDevice, const TextureDescriptor& descriptor, const std::string& pathToTexture)
		: descriptor(descriptor){
		int width = 0;
		int height = 0;
		unsigned char* localBuffer = stbi_load(pathToTexture.c_str(), &width, &height, &BPP, 4); //4 = RGBA

		this->descriptor.dimensions.x = static_cast<uint32_t>(width);
		this->descriptor.dimensions.y = static_cast<uint32_t>(height);

		createTexture(d3dDevice, this->descriptor, localBuffer);

		if(localBuffer){
			stbi_image_free(localBuffer);
		}
	}

	D3DTexture::D3DTexture(ID3D11Device& d3dDevice, const TextureDescriptor& descriptor, const void* data, int32_t BPP)
		: descriptor(descriptor)
		, BPP(BPP){
		createTexture(d3dDevice, descriptor, data);
	}

	D3DTexture::D3DTexture(D3DTexture&& other) noexcept = default;

	D3DTexture& D3DTexture::operator=(D3DTexture&& other) noexcept = default;

	D3DTexture::~D3DTexture() = default;

	const Microsoft::WRL::ComPtr<ID3D11Texture2D>& D3DTexture::getD3DTexture() const{
		return d3dTexture;
	}

	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& D3DTexture::getD3DShaderResourceView() const{
		return d3dResourceView;
	}

	const Microsoft::WRL::ComPtr<ID3D11SamplerState>& D3DTexture::getD3DSampler() const{
		return d3dSampler;
	}

	const TextureDescriptor& D3DTexture::getDescriptor() const{
		return descriptor;
	}

	void D3DTexture::createTexture(ID3D11Device& d3dDevice, const TextureDescriptor& descriptor, const void* data){
		DX11_INFO_PROVIDER;
		
		//If this is being used as a font then the BPP needs to be 1
		if(descriptor.usage == TextureUsage::Font){
			BPP = 1;
		}

		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width				= static_cast<UINT>(descriptor.dimensions.x);
		textureDesc.Height				= static_cast<UINT>(descriptor.dimensions.y);
		textureDesc.MipLevels			= 1u;
		textureDesc.ArraySize			= getArrayElements(descriptor.style) * descriptor.arraySize;
		textureDesc.Format				= getFormatForTexture(descriptor.usage);
		textureDesc.SampleDesc.Count	= 1u;
		textureDesc.SampleDesc.Quality	= 0u;
		textureDesc.Usage				= D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags			= getBindFlags(descriptor.usage);
		textureDesc.CPUAccessFlags		= 0u;
		textureDesc.MiscFlags			= getMiscFlags(descriptor.style);

		if(data){
			CLV_ASSERT(descriptor.arraySize == 1, "DX Textures with array size greater than 1 not supported");

			D3D11_SUBRESOURCE_DATA initialData{};
			initialData.pSysMem				= data;
			initialData.SysMemPitch			= descriptor.dimensions.x * BPP;
			initialData.SysMemSlicePitch	= 0u;

			DX11_THROW_INFO(d3dDevice.CreateTexture2D(&textureDesc, &initialData, &d3dTexture));
		} else{
			DX11_THROW_INFO(d3dDevice.CreateTexture2D(&textureDesc, nullptr, &d3dTexture));
		}

		const D3D11_SHADER_RESOURCE_VIEW_DESC viewDescriptor = createD3DShaderViewDescriptor(descriptor);

		D3D11_SAMPLER_DESC samplerDesc{};
		samplerDesc.Filter		= getFilter(descriptor.filtering);
		samplerDesc.AddressU	= D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV	= D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW	= D3D11_TEXTURE_ADDRESS_CLAMP;

		DX11_THROW_INFO(d3dDevice.CreateShaderResourceView(d3dTexture.Get(), &viewDescriptor, &d3dResourceView));
		DX11_THROW_INFO(d3dDevice.CreateSamplerState(&samplerDesc, &d3dSampler));
	}
}