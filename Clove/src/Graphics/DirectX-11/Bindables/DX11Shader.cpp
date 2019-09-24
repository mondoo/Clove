#include "clvpch.hpp"
#include "DX11Shader.hpp"

#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Graphics/DirectX-11/DX11Exception.hpp"
#include "Graphics/DirectX-11/DX11RenderAPI.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>

namespace clv::gfx{
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

	DX11Shader::DX11Shader(ShaderStyle style){
		initialise(style);
	}

	void DX11Shader::bind(){
		for(const auto& [key, shader] : shaders){
			shader->bind();
		}
	}

	ShaderReflectionData DX11Shader::getReflectionData(){
		ShaderReflectionData outData;
		
		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> reflector;

		if(vertexShader == nullptr){
			CLV_LOG_ERROR("{0}: vertex shader hasn't been initialised, potentially calling function too early!");
			return outData;
		}
		
		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(D3DReflect(vertexShader->getByteCode()->GetBufferPointer(), vertexShader->getByteCode()->GetBufferSize(), IID_ID3D11ShaderReflection, &reflector));

		D3D11_SHADER_DESC shaderDescription; 
		DX11_THROW_INFO(reflector->GetDesc(&shaderDescription));
		
		const UINT inputParamNum = shaderDescription.InputParameters;

		for(UINT i = 0; i < inputParamNum; ++i){
			D3D11_SIGNATURE_PARAMETER_DESC inputParamDescription;
			DX11_THROW_INFO(reflector->GetInputParameterDesc(i, &inputParamDescription));
	
			outData.vertexBufferLayout.add(VertexElement::getTypeFromSemantic(inputParamDescription.SemanticName));
		}

		return outData;
	}

	DX11VertexShader& DX11Shader::getVertexShader(){
		CLV_ASSERT(vertexShader != nullptr, "No vertex shader attached!");
		return *vertexShader;
	}

	void DX11Shader::initialise(ShaderStyle style){
		switch(style){
			case ShaderStyle::Lit:
				{
					auto vs = std::make_unique<DX11VertexShader>(L"Lit-vs.cso");
					vertexShader = vs.get();

					shaders[ShaderType::Vertex] = std::move(vs);
					shaders[ShaderType::Pixel] = std::make_unique<DX11PixelShader>(L"Lit-ps.cso");
				}
				break;

			case ShaderStyle::Unlit:
				{
					auto vs = std::make_unique<DX11VertexShader>(L"Unlit-vs.cso");
					vertexShader = vs.get();

					shaders[ShaderType::Vertex] = std::move(vs);
					shaders[ShaderType::Pixel] = std::make_unique<DX11PixelShader>(L"Unlit-ps.cso");
				}
				break;

			case ShaderStyle::_2D:
				{
					auto vs = std::make_unique<DX11VertexShader>(L"2D-vs.cso");
					vertexShader = vs.get();

					shaders[ShaderType::Vertex] = std::move(vs);
					shaders[ShaderType::Pixel] = std::make_unique<DX11PixelShader>(L"2D-ps.cso");
				}
				break;

			case ShaderStyle::RT:
				{
					auto vs = std::make_unique<DX11VertexShader>(L"RT-vs.cso");
					vertexShader = vs.get();

					shaders[ShaderType::Vertex] = std::move(vs);
					shaders[ShaderType::Pixel] = std::make_unique<DX11PixelShader>(L"RT-ps.cso");
				}
				break;

			case ShaderStyle::Font:
				{
					auto vs = std::make_unique<DX11VertexShader>(L"Font-vs.cso");
					vertexShader = vs.get();

					shaders[ShaderType::Vertex] = std::move(vs);
					shaders[ShaderType::Pixel] = std::make_unique<DX11PixelShader>(L"Font-ps.cso");
				}
				break;

			default:
				CLV_ASSERT(false, "Unknown type! {0}", CLV_FUNCTION_NAME);
				break;
		}
	}

	DX11ShaderElement::DX11ShaderElement() = default;

	DX11ShaderElement::DX11ShaderElement(DX11ShaderElement&& other) noexcept = default;

	DX11ShaderElement& DX11ShaderElement::operator=(DX11ShaderElement&& other) noexcept = default;

	DX11ShaderElement::~DX11ShaderElement() = default;

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