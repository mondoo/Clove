#include "Clove/Graphics/Direct3D/D3DPipelineObject.hpp"

#include "Clove/Graphics/Direct3D/D3DException.hpp"
#include "Clove/Graphics/Direct3D/D3DShader.hpp"
#if CLV_DEBUG
#include "Clove/Graphics/Direct3D/D3DRenderDevice.hpp"
#endif

namespace clv::gfx::d3d{
	D3DPipelineObject::D3DPipelineObject(ID3D11Device& d3dDevice, const std::shared_ptr<Shader>& shader)
		: shader(shader){
		const auto dxShader = std::static_pointer_cast<D3DShader>(shader);

		shaderReflectionData = shader->getReflectionData();
		const auto& layout = shaderReflectionData.vertexBufferLayout;

		std::vector<D3D11_INPUT_ELEMENT_DESC> dxElements;
		dxElements.reserve(layout.count());

		for(int32 i = 0; i < layout.count(); ++i){
			const auto& element = layout.resolve(i);
			const VertexElementType elementType = element.getType();
			const UINT alignmentOffset = (i > 0) ? D3D11_APPEND_ALIGNED_ELEMENT : 0;

			dxElements.push_back({ element.getSemantic(), 0, getDXGIFormatFromType(elementType), 0, alignmentOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}

		auto [vertexByteData, vertexByteSize] = dxShader->getVertexByteData();

		DX11_INFO_PROVIDER;
		DX11_THROW_INFO(d3dDevice.CreateInputLayout(dxElements.data(), static_cast<UINT>(dxElements.size()), vertexByteData, vertexByteSize, &inputLayout));
	}

	D3DPipelineObject::D3DPipelineObject(D3DPipelineObject&& other) = default;

	D3DPipelineObject& D3DPipelineObject::operator=(D3DPipelineObject&& other) = default;

	D3DPipelineObject::~D3DPipelineObject() = default;

	const Microsoft::WRL::ComPtr<ID3D11InputLayout>& D3DPipelineObject::getD3DInputLayout() const{
		return inputLayout;
	}

	const std::shared_ptr<Shader>& D3DPipelineObject::getShader() const{
		return shader;
	}

	const VertexLayout& D3DPipelineObject::getVertexLayout() const{
		return shaderReflectionData.vertexBufferLayout;
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