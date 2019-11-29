#include "Clove/Graphics/Direct3D/D3DShader.hpp"

#include "Clove/Graphics/Direct3D/D3DException.hpp"
#include "Clove/Graphics/Direct3D/ShaderHeaders.hpp"
#if CLV_DEBUG
	#include "Clove/Graphics/Direct3D/D3DRenderDevice.hpp"
#endif

#include <d3d11.h>
#include <d3dcompiler.h>

namespace clv::gfx::d3d{
	D3DShader::D3DShader(ID3D11Device& d3dDevice, const ShaderDescriptor& descriptor)
		: descriptor(descriptor){
		initialise(d3dDevice, descriptor.style);
	}

	D3DShader::D3DShader(D3DShader&& other) noexcept = default;

	D3DShader& D3DShader::operator=(D3DShader&& other) noexcept = default;

	D3DShader::~D3DShader() = default;

	const ShaderDescriptor& D3DShader::getDescriptor() const{
		return descriptor;
	}

	ShaderReflectionData D3DShader::getReflectionData() const{
		ShaderReflectionData outData;

		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> reflector;

		if(d3dVertexShader == nullptr){
			CLV_LOG_ERROR("{0}: vertex shader hasn't been initialised, potentially calling function too early!");
			return outData;
		}

		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(D3DReflect(vertexByteData, vertexByteSize, IID_ID3D11ShaderReflection, &reflector));

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

	const Microsoft::WRL::ComPtr<ID3D11VertexShader>& D3DShader::getD3DVertexShader() const{
		return d3dVertexShader;
	}

	const Microsoft::WRL::ComPtr<ID3D11GeometryShader>& D3DShader::getD3DGeometryShader() const{
		return d3dGeometryShader;
	}

	const Microsoft::WRL::ComPtr<ID3D11PixelShader>& D3DShader::getD3DPixelShader() const{
		return d3dPixelShader;
	}

	std::pair<const BYTE*, SIZE_T> D3DShader::getVertexByteData() const{
		return std::make_pair(vertexByteData, vertexByteSize);
	}

	void D3DShader::initialise(ID3D11Device& d3dDevice, ShaderStyle style){
		DX11_INFO_PROVIDER;

		switch(style){
			case ShaderStyle::Lit_3D:
				vertexByteData = shader_Lit_vs;
				vertexByteSize = sizeof(shader_Lit_vs);

				DX11_THROW_INFO(d3dDevice.CreateVertexShader(vertexByteData, vertexByteSize, nullptr, &d3dVertexShader));
				DX11_THROW_INFO(d3dDevice.CreatePixelShader(shader_Lit_ps, sizeof(shader_Lit_ps), nullptr, &d3dPixelShader));

				break;

			case ShaderStyle::Unlit_3D:
				vertexByteData = shader_Unlit_vs;
				vertexByteSize = sizeof(shader_Unlit_vs);

				DX11_THROW_INFO(d3dDevice.CreateVertexShader(vertexByteData, vertexByteSize, nullptr, &d3dVertexShader));
				DX11_THROW_INFO(d3dDevice.CreatePixelShader(shader_Unlit_ps, sizeof(shader_Unlit_ps), nullptr, &d3dPixelShader));

				break;

			case ShaderStyle::Unlit_2D:
				vertexByteData = shader_2D_vs;
				vertexByteSize = sizeof(shader_2D_vs);

				DX11_THROW_INFO(d3dDevice.CreateVertexShader(vertexByteData, vertexByteSize, nullptr, &d3dVertexShader));
				DX11_THROW_INFO(d3dDevice.CreatePixelShader(shader_2D_ps, sizeof(shader_2D_ps), nullptr, &d3dPixelShader));

				break;

			case ShaderStyle::RT:
				vertexByteData = shader_RT_vs;
				vertexByteSize = sizeof(shader_RT_vs);

				DX11_THROW_INFO(d3dDevice.CreateVertexShader(vertexByteData, vertexByteSize, nullptr, &d3dVertexShader));
				DX11_THROW_INFO(d3dDevice.CreatePixelShader(shader_RT_ps, sizeof(shader_RT_ps), nullptr, &d3dPixelShader));
				break;

			case ShaderStyle::Font:
				vertexByteData = shader_Font_vs;
				vertexByteSize = sizeof(shader_Font_vs);

				DX11_THROW_INFO(d3dDevice.CreateVertexShader(vertexByteData, vertexByteSize, nullptr, &d3dVertexShader));
				DX11_THROW_INFO(d3dDevice.CreatePixelShader(shader_Font_ps, sizeof(shader_Font_ps), nullptr, &d3dPixelShader));
				break;

			case ShaderStyle::CubeShadowMap:
				vertexByteData = shader_CubeShadowMap_vs;
				vertexByteSize = sizeof(shader_CubeShadowMap_vs);

				DX11_THROW_INFO(d3dDevice.CreateVertexShader(vertexByteData, vertexByteSize, nullptr, &d3dVertexShader));
				DX11_THROW_INFO(d3dDevice.CreateGeometryShader(shader_CubeShadowMap_gs, sizeof(shader_CubeShadowMap_gs), nullptr, &d3dGeometryShader));
				DX11_THROW_INFO(d3dDevice.CreatePixelShader(shader_CubeShadowMap_ps, sizeof(shader_CubeShadowMap_ps), nullptr, &d3dPixelShader));
				break;

			default:
				CLV_ASSERT(false, "Unknown type! {0}", CLV_FUNCTION_NAME);
				break;
		}
	}
}