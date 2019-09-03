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
		return reflectionData;

		//ShaderReflectionData outData;
		//
		//Microsoft::WRL::ComPtr<ID3D11ShaderReflection> reflector;

		////TODO: warn if vertex shader is nullptr?
		//DX11_INFO_PROVIDER;
		//DX11_THROW_INFO(D3DReflect(vertexShader->getByteCode()->GetBufferPointer(), vertexShader->getByteCode()->GetBufferSize(), IID_ID3D11ShaderReflection, &reflector));

		//D3D11_SHADER_DESC shaderDescription; 
		//DX11_THROW_INFO(reflector->GetDesc(&shaderDescription));
		//
		//const UINT inputParamNum = shaderDescription.InputParameters;

		//for(UINT i = 0; i < inputParamNum; ++i){
		//	D3D11_SIGNATURE_PARAMETER_DESC inputParamDescription;
		//	DX11_THROW_INFO(reflector->GetInputParameterDesc(i, &inputParamDescription));
	
		//	outData.vertexBufferLayout.add(VertexElement::getTypeFromSemantic(inputParamDescription.SemanticName));
		//}

		//const UINT constantBuffers = shaderDescription.ConstantBuffers;
		//for(UINT i = 0; i < constantBuffers; ++i){
		//	ID3D11ShaderReflectionConstantBuffer* constantBuffer = reflector->GetConstantBufferByIndex(i);
		//	D3D11_SHADER_BUFFER_DESC bufferDescription;
		//	DX11_THROW_INFO(constantBuffer->GetDesc(&bufferDescription));
		//	const UINT variables = bufferDescription.Variables;

		//	for(UINT j = 0; j < variables; ++j){
		//		ID3D11ShaderReflectionVariable* variable = constantBuffer->GetVariableByIndex(j);
		//		D3D11_SHADER_VARIABLE_DESC variableDescription;
		//		DX11_THROW_INFO(variable->GetDesc(&variableDescription));

		//		ID3D11ShaderReflectionType* variableType = variable->GetType();
		//		D3D11_SHADER_TYPE_DESC typeDescription;
		//		DX11_THROW_INFO(variableType->GetDesc(&typeDescription));
		//	}
		//}

		//return outData;
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

			default:
				CLV_ASSERT(false, "Unknown type! {0}", __func__);
				break;
		}
	}

	void DX11Shader::reflectVertexShader(ID3DBlob* byteCode){
		//ShaderReflectionData outData;

		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> reflector;

		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(D3DReflect(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), IID_ID3D11ShaderReflection, &reflector));

		D3D11_SHADER_DESC shaderDescription;
		DX11_THROW_INFO(reflector->GetDesc(&shaderDescription));

		const UINT inputParamNum = shaderDescription.InputParameters;

		for(UINT i = 0; i < inputParamNum; ++i){
			D3D11_SIGNATURE_PARAMETER_DESC inputParamDescription;
			DX11_THROW_INFO(reflector->GetInputParameterDesc(i, &inputParamDescription));

			reflectionData.vertexBufferLayout.add(VertexElement::getTypeFromSemantic(inputParamDescription.SemanticName));
		}

		//TODO: set up the static casts

		const UINT constantBuffers = shaderDescription.ConstantBuffers;
		for(UINT i = 0; i < constantBuffers; ++i){
			ShaderBufferDescription shaderBufferDescription;
			
			ID3D11ShaderReflectionConstantBuffer* constantBuffer = reflector->GetConstantBufferByIndex(i);
			D3D11_SHADER_BUFFER_DESC bufferDescription;
			DX11_THROW_INFO(constantBuffer->GetDesc(&bufferDescription));

			const UINT variables = bufferDescription.Variables;
			
			shaderBufferDescription.name = bufferDescription.Name;
			//shaderBufferDescription.bindingPoint = //no bp?
			shaderBufferDescription.totalSize = bufferDescription.Size;
			shaderBufferDescription.variables.reserve(variables);

			for(UINT j = 0; j < variables; ++j){
				ID3D11ShaderReflectionVariable* variable = constantBuffer->GetVariableByIndex(j);
				D3D11_SHADER_VARIABLE_DESC variableDescription;
				DX11_THROW_INFO(variable->GetDesc(&variableDescription));

				ID3D11ShaderReflectionType* variableType = variable->GetType();
				D3D11_SHADER_TYPE_DESC typeDescription;
				DX11_THROW_INFO(variableType->GetDesc(&typeDescription));

				ShaderBufferVariable bufferVariable;
				bufferVariable.name = variableDescription.Name;
				bufferVariable.size = variableDescription.Size;
				//bufferVariable.type = typeDescription. //TODO: Type
			}

			reflectionData.bufferDescriptions.push_back(shaderBufferDescription);
		}

		/*return outData;*/
	}

	void DX11Shader::reflectPixelShader(ID3DBlob* byteCode){
		//TODO:
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