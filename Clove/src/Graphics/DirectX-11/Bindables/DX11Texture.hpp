#pragma once

#include "Clove/Graphics/Bindables/Texture.hpp"

#include <wrl.h>

struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;

namespace clv::gfx{
	class Renderer;

	class DX11Texture : public Texture{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
		
		std::string filePath;

		int width = 0;
		int height = 0;
		int BPP = 0;

		//FUNCTIONS
	public:
		DX11Texture() = delete;
		DX11Texture(const DX11Texture& other) = delete;
		DX11Texture(DX11Texture&& other) noexcept;
		DX11Texture& operator=(const DX11Texture& other) = delete;
		DX11Texture& operator=(DX11Texture&& other) noexcept;
		virtual ~DX11Texture();

		DX11Texture(const std::string& filePath);

		virtual void bind(Renderer& renderer) override;
		virtual void unbind() override;

		virtual int getWidth() const override;
		virtual int getHeight() const override;
	};
}
