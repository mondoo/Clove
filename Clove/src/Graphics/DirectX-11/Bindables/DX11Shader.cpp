#include "clvpch.hpp"
#include "DX11Shader.hpp"

#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Graphics/DirectX-11/DX11Exception.hpp"
#include "Graphics/DirectX-11/DX11RenderAPI.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>

namespace clv::gfx{
	DX11Shader::DX11Shader() = default;

	DX11Shader::DX11Shader(DX11Shader&& other) noexcept{
		shaders = std::move(other.shaders);
		vertexShader = other.vertexShader;
	}

	DX11Shader& DX11Shader::operator=(DX11Shader&& other) noexcept{
		shaders = std::move(other.shaders);
		vertexShader = other.vertexShader;

		return *this;
	}

	DX11Shader::~DX11Shader() = default;

	void DX11Shader::bind(){
		for(const auto& [key, shader] : shaders){
			shader->bind();
		}
	}

	void DX11Shader::attachShader(ShaderType type){
		switch(type){
			case ShaderType::Vertex:
				{
					auto vs = std::make_unique<DX11VertexShader>(L"Lit-vs.cso");
					vertexShader = vs.get();
					shaders[type] = std::move(vs);
				}
				break;
			case ShaderType::Pixel:
				shaders[type] = std::make_unique<DX11PixelShader>(L"Lit-ps.cso");
				break;

			case ShaderType::Vertex2D:
				{
					auto vs = std::make_unique<DX11VertexShader>(L"2D-vs.cso");
					vertexShader = vs.get();
					shaders[type] = std::move(vs);
				}
				break;
			case ShaderType::Pixel2D:
				shaders[type] = std::make_unique<DX11PixelShader>(L"2D-ps.cso");
				break;

			default:
				CLV_ASSERT(false, "Unknown type! " __FUNCTION__);
				break;
		}
	}

	DX11VertexShader& DX11Shader::getVertexShader(){
		CLV_ASSERT(vertexShader != nullptr, "No vertex shader attached!");
		return *vertexShader;
	}

	DX11VertexShader::DX11VertexShader(DX11VertexShader&& other) noexcept = default;

	DX11VertexShader& DX11VertexShader::operator=(DX11VertexShader&& other) noexcept = default;

	DX11VertexShader::~DX11VertexShader() = default;

	DX11VertexShader::DX11VertexShader(const std::wstring& path){
		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(D3DReadFileToBlob(path.c_str(), &byteCode));
		DX11_THROW_INFO(DX11RenderAPI::getDevice().CreateVertexShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &vertexShader));
	}

	void DX11VertexShader::bind(){
		DX11RenderAPI::getContext().VSSetShader(vertexShader.Get(), nullptr, 0u);
	}

	ID3DBlob* DX11VertexShader::getByteCode() const{
		return byteCode.Get();
	}

	DX11PixelShader::DX11PixelShader(DX11PixelShader&& other) noexcept = default;

	DX11PixelShader& DX11PixelShader::operator=(DX11PixelShader&& other) noexcept = default;

	DX11PixelShader::~DX11PixelShader() = default;

	DX11PixelShader::DX11PixelShader(const std::wstring& path){
		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(D3DReadFileToBlob(path.c_str(), &byteCode));
		DX11_THROW_INFO(DX11RenderAPI::getDevice().CreatePixelShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &pixelShader));
	}

	void DX11PixelShader::bind(){
		DX11RenderAPI::getContext().PSSetShader(pixelShader.Get(), nullptr, 0u);
	}

	ID3DBlob* DX11PixelShader::getByteCode() const{
		return byteCode.Get();
	}
}