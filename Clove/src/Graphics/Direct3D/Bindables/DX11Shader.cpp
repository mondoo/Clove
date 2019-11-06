#include "DX11Shader.hpp"

#include "Graphics/DirectX-11/DX11Exception.hpp"
#include "Graphics/DirectX-11/DX11RenderAPI.hpp"
#include "Graphics/DirectX-11/ShaderHeaders.hpp"

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
		DX11_THROW_INFO(D3DReflect(vertexShader->getBytePointer(), vertexShader->getByteSize(), IID_ID3D11ShaderReflection, &reflector));

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
					auto vs = std::make_unique<DX11VertexShader>(shader_Lit_vs, sizeof(shader_Lit_vs));
					vertexShader = vs.get();

					shaders[ShaderType::Vertex] = std::move(vs);
					shaders[ShaderType::Pixel] = std::make_unique<DX11PixelShader>(shader_Lit_ps, sizeof(shader_Lit_ps));
				}
				break;

			case ShaderStyle::Unlit:
				{
					auto vs = std::make_unique<DX11VertexShader>(shader_Unlit_vs, sizeof(shader_Unlit_vs));
					vertexShader = vs.get();

					shaders[ShaderType::Vertex] = std::move(vs);
					shaders[ShaderType::Pixel] = std::make_unique<DX11PixelShader>(shader_Unlit_ps, sizeof(shader_Unlit_ps));
				}
				break;

			case ShaderStyle::_2D:
				{
					auto vs = std::make_unique<DX11VertexShader>(shader_2D_vs, sizeof(shader_2D_vs));
					vertexShader = vs.get();

					shaders[ShaderType::Vertex] = std::move(vs);
					shaders[ShaderType::Pixel] = std::make_unique<DX11PixelShader>(shader_2D_ps, sizeof(shader_2D_ps));
				}
				break;

			case ShaderStyle::RT:
				{
					auto vs = std::make_unique<DX11VertexShader>(shader_RT_vs, sizeof(shader_RT_vs));
					vertexShader = vs.get();

					shaders[ShaderType::Vertex] = std::move(vs);
					shaders[ShaderType::Pixel] = std::make_unique<DX11PixelShader>(shader_RT_ps, sizeof(shader_RT_ps));
				}
				break;

			case ShaderStyle::Font:
				{
					auto vs = std::make_unique<DX11VertexShader>(shader_Font_vs, sizeof(shader_Font_vs));
					vertexShader = vs.get();

					shaders[ShaderType::Vertex] = std::move(vs);
					shaders[ShaderType::Pixel] = std::make_unique<DX11PixelShader>(shader_Font_ps, sizeof(shader_Font_ps));
				}
				break;

			case ShaderStyle::CubeShadowMap:
				{
					auto vs = std::make_unique<DX11VertexShader>(shader_CubeShadowMap_vs, sizeof(shader_CubeShadowMap_vs));
					vertexShader = vs.get();

					shaders[ShaderType::Vertex] = std::move(vs);
					shaders[ShaderType::Pixel] = std::make_unique<DX11PixelShader>(shader_CubeShadowMap_ps, sizeof(shader_CubeShadowMap_ps));
					shaders[ShaderType::Geometry] = std::make_unique<DX11GeometryShader>(shader_CubeShadowMap_gs, sizeof(shader_CubeShadowMap_gs));
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

	DX11VertexShader::DX11VertexShader(const BYTE* shaderByteData, SIZE_T shaderByteSize){
		this->shaderByteData = shaderByteData;
		this->shaderByteSize = shaderByteSize;
		
		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(DX11RenderAPI::getDevice().CreateVertexShader(shaderByteData, shaderByteSize, nullptr, &vertexShader));
	}

	void DX11VertexShader::bind(){
		DX11RenderAPI::getContext().VSSetShader(vertexShader.Get(), nullptr, 0u);
	}

	const BYTE* DX11VertexShader::getBytePointer() const{
		return shaderByteData;
	}

	SIZE_T DX11VertexShader::getByteSize() const{
		return shaderByteSize;
	}

	DX11PixelShader::DX11PixelShader(DX11PixelShader&& other) noexcept = default;

	DX11PixelShader& DX11PixelShader::operator=(DX11PixelShader&& other) noexcept = default;

	DX11PixelShader::~DX11PixelShader() = default;

	DX11PixelShader::DX11PixelShader(const BYTE* shaderByteData, SIZE_T shaderByteSize){
		this->shaderByteData = shaderByteData;
		this->shaderByteSize = shaderByteSize;

		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(DX11RenderAPI::getDevice().CreatePixelShader(shaderByteData, shaderByteSize, nullptr, &pixelShader));
	}

	void DX11PixelShader::bind(){
		DX11RenderAPI::getContext().PSSetShader(pixelShader.Get(), nullptr, 0u);
	}

	const BYTE* DX11PixelShader::getBytePointer() const{
		return shaderByteData;
	}

	SIZE_T DX11PixelShader::getByteSize() const{
		return shaderByteSize;
	}

	DX11GeometryShader::DX11GeometryShader(DX11GeometryShader&& other) noexcept = default;

	DX11GeometryShader& DX11GeometryShader::operator=(DX11GeometryShader&& other) noexcept = default;

	DX11GeometryShader::~DX11GeometryShader() = default;

	DX11GeometryShader::DX11GeometryShader(const BYTE* shaderByteData, SIZE_T shaderByteSize){
		this->shaderByteData = shaderByteData;
		this->shaderByteSize = shaderByteSize;

		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(DX11RenderAPI::getDevice().CreateGeometryShader(shaderByteData, shaderByteSize, nullptr, &geometryShader));
	}

	void DX11GeometryShader::bind(){
		DX11RenderAPI::getContext().GSSetShader(geometryShader.Get(), nullptr, 0u);
	}

	const BYTE* DX11GeometryShader::getBytePointer() const{
		return shaderByteData;
	}

	SIZE_T DX11GeometryShader::getByteSize() const{
		return shaderByteSize;
	}
}