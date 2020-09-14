#pragma once

#include "Clove/Graphics/Shader.hpp"

#include <wrl.h>
#include <d3d11.h>
#include <span>

namespace clv::gfx::d3d{
	class D3DShader : public Shader{
		//VARIABLES
	private:
		std::shared_ptr<GraphicsFactory> factory;

		ShaderDescriptor descriptor;

		Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;

		//FUNCTIONS	
	public:
		D3DShader() = delete;
		D3DShader(std::shared_ptr<GraphicsFactory> factory, ID3D11Device& d3dDevice, ShaderDescriptor descriptor, std::string_view pathToShader);
        D3DShader(std::shared_ptr<GraphicsFactory> factory, ID3D11Device& d3dDevice, ShaderDescriptor descriptor, std::span<const std::byte> sourceBytes);

		D3DShader(const D3DShader& other) = delete;
		D3DShader(D3DShader&& other) noexcept;

		D3DShader& operator=(const D3DShader& other) = delete;
		D3DShader& operator=(D3DShader&& other) noexcept;

		~D3DShader();

		const std::shared_ptr<GraphicsFactory>& getFactory() const override;

		const ShaderDescriptor& getDescriptor() const override;

		const Microsoft::WRL::ComPtr<ID3DBlob>& getShaderBlob() const;
	};
}