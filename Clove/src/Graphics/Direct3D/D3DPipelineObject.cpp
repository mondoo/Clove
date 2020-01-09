#include "Clove/Graphics/Direct3D/D3DPipelineObject.hpp"

#include "Clove/Graphics/Direct3D/D3DException.hpp"
#include "Clove/Graphics/Direct3D/D3DShader.hpp"

namespace clv::gfx::d3d{
	D3DPipelineObject::D3DPipelineObject(ID3D11Device& d3dDevice, const std::shared_ptr<Shader>& shader)
		: shader(shader){
		const auto d3dShader = std::static_pointer_cast<D3DShader>(shader);

		shaderReflectionData = d3dShader->getReflectionData();
		const auto& layout = shaderReflectionData.vertexBufferLayout;

		std::vector<D3D11_INPUT_ELEMENT_DESC> d3dElements;
		d3dElements.reserve(layout.count());

		for(int32 i = 0; i < layout.count(); ++i){
			const auto& element = layout.resolve(i);
			const VertexElementType elementType = element.getType();
			const UINT alignmentOffset = (i > 0) ? D3D11_APPEND_ALIGNED_ELEMENT : 0;

			d3dElements.push_back({ element.getSemantic(), 0, getDXGIFormatFromType(elementType), 0, alignmentOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}

		auto [vertexByteData, vertexByteSize] = d3dShader->getVertexByteData();

		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(d3dDevice.CreateInputLayout(d3dElements.data(), static_cast<UINT>(d3dElements.size()), vertexByteData, vertexByteSize, &inputLayout));
	}

	D3DPipelineObject::D3DPipelineObject(D3DPipelineObject&& other) = default;

	D3DPipelineObject& D3DPipelineObject::operator=(D3DPipelineObject&& other) = default;

	D3DPipelineObject::~D3DPipelineObject() = default;

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

	const std::shared_ptr<Shader>& D3DPipelineObject::getShader() const{
		return shader;
	}

	const VertexLayout& D3DPipelineObject::getVertexLayout() const{
		return shaderReflectionData.vertexBufferLayout;
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
			default:
				CLV_ASSERT(false, "Invalid element type");
				return DXGI_FORMAT_R32G32B32_FLOAT;
		}
	}
}
