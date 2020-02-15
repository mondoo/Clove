#include "Clove/Graphics/Direct3D/D3DPipelineObject.hpp"

#include "Clove/Graphics/Direct3D/D3DException.hpp"
#include "Clove/Graphics/Direct3D/D3DShader.hpp"

#include <d3dcompiler.h>

namespace clv::gfx::d3d{
	D3DPipelineObject::D3DPipelineObject(const Microsoft::WRL::ComPtr<ID3D11Device>& d3dDevice)
		: d3dDevice(d3dDevice){
		setBlendState(true);
		setCullMode(CullFace::Back, true);
	}

	D3DPipelineObject::D3DPipelineObject(D3DPipelineObject&& other) = default;

	D3DPipelineObject& D3DPipelineObject::operator=(D3DPipelineObject&& other) = default;

	D3DPipelineObject::~D3DPipelineObject() = default;

	void D3DPipelineObject::setVertexShader(const Shader& vertexShader){
		DX11_INFO_PROVIDER;

		const D3DShader& d3dShader = static_cast<const D3DShader&>(vertexShader);
		const auto& shaderBlob = d3dShader.getShaderBlob();
		LPVOID byteCode = shaderBlob->GetBufferPointer();
		SIZE_T byteCodeSize = shaderBlob->GetBufferSize();

		DX11_THROW_INFO(d3dDevice->CreateVertexShader(byteCode, byteCodeSize, nullptr, &d3dVertexShader));

		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> reflector;

		DX11_THROW_INFO(D3DReflect(byteCode, byteCodeSize, IID_ID3D11ShaderReflection, &reflector));

		D3D11_SHADER_DESC shaderDescription;
		DX11_THROW_INFO(reflector->GetDesc(&shaderDescription));

		vertexLayout.clear();
		const UINT inputParamNum = shaderDescription.InputParameters;
		for(UINT i = 0; i < inputParamNum; ++i){
			D3D11_SIGNATURE_PARAMETER_DESC inputParamDescription;
			DX11_THROW_INFO(reflector->GetInputParameterDesc(i, &inputParamDescription));

			vertexLayout.add(VertexElement::getTypeFromSemantic(inputParamDescription.SemanticName));
		}

		std::vector<D3D11_INPUT_ELEMENT_DESC> d3dElements;
		d3dElements.reserve(vertexLayout.count());

		for(int32_t i = 0; i < vertexLayout.count(); ++i){
			const auto& element = vertexLayout.resolve(i);
			const VertexElementType elementType = element.getType();
			const UINT alignmentOffset = (i > 0) ? D3D11_APPEND_ALIGNED_ELEMENT : 0;

			d3dElements.push_back({ element.getSemantic().data(), 0, getDXGIFormatFromType(elementType), 0, alignmentOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}

		DX11_THROW_INFO(d3dDevice->CreateInputLayout(d3dElements.data(), static_cast<UINT>(d3dElements.size()), byteCode, byteCodeSize, &inputLayout));
	}

	void D3DPipelineObject::setGeometryShader(const Shader& geometryShader){
		DX11_INFO_PROVIDER;

		const D3DShader& d3dShader = static_cast<const D3DShader&>(geometryShader);

		const auto& shaderBlob = d3dShader.getShaderBlob();
		DX11_THROW_INFO(d3dDevice->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &d3dGeometryShader));
	}

	void D3DPipelineObject::setPixelShader(const Shader& pixelShader){
		DX11_INFO_PROVIDER;

		const D3DShader& d3dShader = static_cast<const D3DShader&>(pixelShader);

		const auto& shaderBlob = d3dShader.getShaderBlob();
		DX11_THROW_INFO(d3dDevice->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &d3dPixelShader));
	}

	void D3DPipelineObject::setBlendState(bool enabled){
		blendDesc = {};
		blendDesc.AlphaToCoverageEnable					= FALSE;
		blendDesc.IndependentBlendEnable				= FALSE;
		blendDesc.RenderTarget[0].BlendEnable			= enabled ? TRUE : FALSE;
		blendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	void D3DPipelineObject::setCullMode(CullFace face, bool frontFaceCounterClockwise){
		rasteriserDesc = {};
		rasteriserDesc.FillMode					= D3D11_FILL_SOLID;
		rasteriserDesc.CullMode					= face == CullFace::Back ? D3D11_CULL_BACK : D3D11_CULL_FRONT;
		rasteriserDesc.FrontCounterClockwise	= frontFaceCounterClockwise ? TRUE : FALSE;
		rasteriserDesc.DepthClipEnable			= TRUE;
	}

	const VertexLayout& D3DPipelineObject::getVertexLayout() const{
		return vertexLayout;
	}

	const Microsoft::WRL::ComPtr<ID3D11VertexShader>& D3DPipelineObject::getD3DVertexShader() const{
		return d3dVertexShader;
	}

	const Microsoft::WRL::ComPtr<ID3D11GeometryShader>& D3DPipelineObject::getD3DGeometryShader() const{
		return d3dGeometryShader;
	}

	const Microsoft::WRL::ComPtr<ID3D11PixelShader>& D3DPipelineObject::getD3DPixelShader() const{
		return d3dPixelShader;
	}

	const Microsoft::WRL::ComPtr<ID3D11InputLayout>& D3DPipelineObject::getD3DInputLayout() const{
		return inputLayout;
	}

	const D3D11_BLEND_DESC& D3DPipelineObject::getD3DBlendDesc() const{
		return blendDesc;
	}

	const D3D11_RASTERIZER_DESC& D3DPipelineObject::getD3DRasterDesc() const{
		return rasteriserDesc;
	}

	DXGI_FORMAT D3DPipelineObject::getDXGIFormatFromType(VertexElementType type){
		switch(type){
			case VertexElementType::position2D:
				return DXGI_FORMAT_R32G32_FLOAT;
			case VertexElementType::position3D:
				return DXGI_FORMAT_R32G32B32_FLOAT;
			case VertexElementType::texture2D:
				return DXGI_FORMAT_R32G32_FLOAT;
			case VertexElementType::normal:
				return DXGI_FORMAT_R32G32B32_FLOAT;
			case VertexElementType::colour3D:
				return DXGI_FORMAT_R32G32B32_FLOAT;
			default:
				CLV_ASSERT(false, "Invalid element type");
				return DXGI_FORMAT_R32G32B32_FLOAT;
		}
	}
}
