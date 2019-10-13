#pragma once

#include "Clove/Graphics/Bindables/Texture.hpp"

#include <wrl.h>

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;

namespace clv::gfx{
	class Renderer;

	class DX11Texture : public Texture{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
		
		std::string filePath;
		TextureUsage usage = TextureUsage::Default;
		TextureStyle style = TextureStyle::Default;

		int32 width = 0;
		int32 height = 0;
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

		DX11Texture(const std::string& filePath, uint32 bindingPoint, TextureStyle style);
		DX11Texture(int32 width, int32 height, TextureUsage usageType, uint32 bindingPoint, TextureStyle style);

		virtual void bind() override;

		virtual int32 getWidth() const override;
		virtual int32 getHeight() const override;

		virtual TextureBindingPoint getBindingPoint() const override;
		virtual TextureUsage getUsageType() const override;
		virtual TextureStyle getTextureStyle() const override;

		const Microsoft::WRL::ComPtr<ID3D11Texture2D>& getTexture() const;

	private:
		void createTexture(TextureUsage usageType, TextureStyle styleType, void* pixels);
	};
}
