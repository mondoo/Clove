#pragma once

#include "Clove/Graphics/Bindables/Texture.hpp"

#include <wrl.h>
#include <d3d11.h>

namespace clv::gfx{
	class Renderer;

	class DX11Texture : public Texture{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
		
		std::string filePath = "";

		TextureDescriptor descriptor = {};

		int32 BPP = 0;

		uint32 bindingPoint = 0;

		//FUNCTIONS
	public:
		DX11Texture() = delete;
		DX11Texture(const DX11Texture& other) = delete;
		DX11Texture(DX11Texture&& other) noexcept;
		DX11Texture& operator=(const DX11Texture& other) = delete;
		DX11Texture& operator=(DX11Texture&& other) noexcept;
		virtual ~DX11Texture();

		DX11Texture(const std::string& filePath, uint32 bindingPoint);
		DX11Texture(void* bufferData, uint32 bindingPoint, const gfx::TextureDescriptor& descriptor);
		DX11Texture(uint32 bindingPoint, const gfx::TextureDescriptor& descriptor);

		virtual void bind() override;

		virtual uint32 getWidth() const override;
		virtual uint32 getHeight() const override;

		virtual TextureBindingPoint getBindingPoint() const override;
		virtual TextureUsage getUsageType() const override;
		virtual TextureStyle getTextureStyle() const override;

		const Microsoft::WRL::ComPtr<ID3D11Texture2D>& getTexture() const;

	private:
		void createTexture(void* pixels);

		D3D11_TEXTURE2D_DESC createD3DTextureDescriptor() const;
		D3D11_SHADER_RESOURCE_VIEW_DESC createD3DShaderViewDescriptor() const;
		D3D11_SAMPLER_DESC createD3DSamplerDescriptor() const;

		UINT getBindFlags(const TextureUsage usage) const;
		DXGI_FORMAT getFormatForTexture(const TextureUsage usage) const;
		DXGI_FORMAT getFormatForShaderView(const TextureUsage usage) const;

		UINT getArrayElements(const TextureStyle style) const;
		D3D_SRV_DIMENSION getViewDimension(const TextureStyle style, const uint8 arraySize) const;

		UINT getMiscFlags(const TextureStyle style) const;
	};
}
