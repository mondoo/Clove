#include "D3DShader.hpp"

#include "Graphics/Direct3D/D3DException.hpp"
#include "Graphics/Direct3D/ShaderHeaders.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>

namespace clv::gfx::d3d::_11{
	D3DShader::D3DShader(ID3D11Device& d3dDevice, const ShaderDescriptor& descriptor)
		: descriptor(descriptor){
		initialise(d3dDevice, descriptor.style);
	}

	D3DShader::D3DShader(D3DShader&& other) noexcept{
		d3dShaders = std::move(other.d3dShaders);
		vertexShader = other.vertexShader;
	}

	D3DShader& D3DShader::operator=(D3DShader&& other) noexcept{
		d3dShaders = std::move(other.d3dShaders);
		vertexShader = other.vertexShader;

		return *this;
	}

	D3DShader::~D3DShader() = default;

	const ShaderDescriptor& D3DShader::getDescriptor() const{
		return descriptor;
	}

	ShaderReflectionData D3DShader::getReflectionData() const{
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

	const std::unordered_map<ShaderType, std::unique_ptr<D3DShaderElement>>& D3DShader::getD3DShaders() const{
		return d3dShaders;
	}

	void D3DShader::initialise(ID3D11Device& d3dDevice, ShaderStyle style){
		switch(style){
			case ShaderStyle::Lit_3D:
				{
					auto vs = std::make_unique<D3DVertexShader>(shader_Lit_vs, sizeof(shader_Lit_vs));
					vertexShader = vs.get();

					d3dShaders[ShaderType::Vertex] = std::move(vs);
					d3dShaders[ShaderType::Pixel] = std::make_unique<D3DPixelShader>(shader_Lit_ps, sizeof(shader_Lit_ps));
				}
				break;

			case ShaderStyle::Unlit_3D:
				{
					auto vs = std::make_unique<D3DVertexShader>(shader_Unlit_vs, sizeof(shader_Unlit_vs));
					vertexShader = vs.get();

					d3dShaders[ShaderType::Vertex] = std::move(vs);
					d3dShaders[ShaderType::Pixel] = std::make_unique<D3DPixelShader>(shader_Unlit_ps, sizeof(shader_Unlit_ps));
				}
				break;

			case ShaderStyle::Unlit_2D:
				{
					auto vs = std::make_unique<D3DVertexShader>(shader_2D_vs, sizeof(shader_2D_vs));
					vertexShader = vs.get();

					d3dShaders[ShaderType::Vertex] = std::move(vs);
					d3dShaders[ShaderType::Pixel] = std::make_unique<D3DPixelShader>(shader_2D_ps, sizeof(shader_2D_ps));
				}
				break;

			case ShaderStyle::RT:
				{
					auto vs = std::make_unique<D3DVertexShader>(shader_RT_vs, sizeof(shader_RT_vs));
					vertexShader = vs.get();

					d3dShaders[ShaderType::Vertex] = std::move(vs);
					d3dShaders[ShaderType::Pixel] = std::make_unique<D3DPixelShader>(shader_RT_ps, sizeof(shader_RT_ps));
				}
				break;

			case ShaderStyle::Font:
				{
					auto vs = std::make_unique<D3DVertexShader>(shader_Font_vs, sizeof(shader_Font_vs));
					vertexShader = vs.get();

					d3dShaders[ShaderType::Vertex] = std::move(vs);
					d3dShaders[ShaderType::Pixel] = std::make_unique<D3DPixelShader>(shader_Font_ps, sizeof(shader_Font_ps));
				}
				break;

			case ShaderStyle::CubeShadowMap:
				{
					auto vs = std::make_unique<D3DVertexShader>(shader_CubeShadowMap_vs, sizeof(shader_CubeShadowMap_vs));
					vertexShader = vs.get();

					d3dShaders[ShaderType::Vertex] = std::move(vs);
					d3dShaders[ShaderType::Pixel] = std::make_unique<D3DPixelShader>(shader_CubeShadowMap_ps, sizeof(shader_CubeShadowMap_ps));
					d3dShaders[ShaderType::Geometry] = std::make_unique<D3DGeometryShader>(shader_CubeShadowMap_gs, sizeof(shader_CubeShadowMap_gs));
				}
				break;

			default:
				CLV_ASSERT(false, "Unknown type! {0}", CLV_FUNCTION_NAME);
				break;
		}
	}

	D3DVertexShader::D3DVertexShader(ID3D11Device& d3dDevice, const BYTE* shaderByteData, SIZE_T shaderByteSize){
		this->shaderByteData = shaderByteData;
		this->shaderByteSize = shaderByteSize;

		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(d3dDevice.CreateVertexShader(shaderByteData, shaderByteSize, nullptr, &vertexShader));
	}

	D3DVertexShader::D3DVertexShader(D3DVertexShader&& other) noexcept = default;

	D3DVertexShader& D3DVertexShader::operator=(D3DVertexShader&& other) noexcept = default;

	D3DVertexShader::~D3DVertexShader() = default;

	const BYTE* D3DVertexShader::getBytePointer() const{
		return shaderByteData;
	}

	SIZE_T D3DVertexShader::getByteSize() const{
		return shaderByteSize;
	}

	D3DPixelShader::D3DPixelShader(ID3D11Device& d3dDevice, const BYTE* shaderByteData, SIZE_T shaderByteSize){
		this->shaderByteData = shaderByteData;
		this->shaderByteSize = shaderByteSize;

		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(d3dDevice.CreatePixelShader(shaderByteData, shaderByteSize, nullptr, &pixelShader));
	}

	D3DPixelShader::D3DPixelShader(D3DPixelShader&& other) noexcept = default;

	D3DPixelShader& D3DPixelShader::operator=(D3DPixelShader&& other) noexcept = default;

	D3DPixelShader::~D3DPixelShader() = default;

	const BYTE* D3DPixelShader::getBytePointer() const{
		return shaderByteData;
	}

	SIZE_T D3DPixelShader::getByteSize() const{
		return shaderByteSize;
	}

	D3DGeometryShader::D3DGeometryShader(ID3D11Device& d3dDevice, const BYTE* shaderByteData, SIZE_T shaderByteSize){
		this->shaderByteData = shaderByteData;
		this->shaderByteSize = shaderByteSize;

		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(d3dDevice.CreateGeometryShader(shaderByteData, shaderByteSize, nullptr, &geometryShader));
	}

	D3DGeometryShader::D3DGeometryShader(D3DGeometryShader&& other) noexcept = default;

	D3DGeometryShader& D3DGeometryShader::operator=(D3DGeometryShader&& other) noexcept = default;

	D3DGeometryShader::~D3DGeometryShader() = default;

	const BYTE* D3DGeometryShader::getBytePointer() const{
		return shaderByteData;
	}

	SIZE_T D3DGeometryShader::getByteSize() const{
		return shaderByteSize;
	}
}