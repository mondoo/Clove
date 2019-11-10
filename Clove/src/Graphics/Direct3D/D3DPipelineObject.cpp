#include "D3DPipelineObject.hpp"

namespace clv::gfx::d3d::_11{
	D3DPipelineObject::D3DPipelineObject(const std::shared_ptr<Shader>& shader)
		: shader(shader){
		shaderReflectionData = shader->getReflectionData();
	}

	D3DPipelineObject::D3DPipelineObject(D3DPipelineObject&& other) = default;

	D3DPipelineObject& D3DPipelineObject::operator=(D3DPipelineObject&& other) = default;

	D3DPipelineObject::~D3DPipelineObject() = default;

	const std::shared_ptr<Shader>& D3DPipelineObject::getShader() const{
		return shader;
	}

	const VertexLayout& D3DPipelineObject::getVertexLayout() const{
		return shaderReflectionData.vertexBufferLayout;
	}
}