#pragma once

#include "Clove/Graphics/Core/Shader.hpp"

#include <wrl.h>
#include <d3d11.h>

namespace clv::gfx::d3d{
	class D3DShader : public Shader{
		//VARIABLES
	private:
		ShaderDescriptor descriptor;
		Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;

		//FUNCTIONS	
	public:
		D3DShader() = delete;
		D3DShader(ID3D11Device& d3dDevice, const ShaderDescriptor& descriptor, std::string_view pathToShader);

		D3DShader(const D3DShader& other) = delete;
		D3DShader(D3DShader&& other) noexcept;

		D3DShader& operator=(const D3DShader& other) = delete;
		D3DShader& operator=(D3DShader&& other) noexcept;

		virtual ~D3DShader();

		virtual const ShaderDescriptor& getDescriptor() const override;

		const Microsoft::WRL::ComPtr<ID3DBlob>& getShaderBlob() const;

	private:
		LPCSTR getShaderProfile(const ShaderStage stage);
	};
}