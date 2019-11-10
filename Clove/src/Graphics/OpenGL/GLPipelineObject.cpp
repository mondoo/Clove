#include "GLPipelineObject.hpp"

namespace clv::gfx::ogl{
	GLPipelineObject::GLPipelineObject(const std::shared_ptr<Shader>& shader)
		: shader(shader){
		shaderReflectionData = shader->getReflectionData();
	}

	GLPipelineObject::GLPipelineObject(GLPipelineObject&& other) = default;

	GLPipelineObject& GLPipelineObject::operator=(GLPipelineObject&& other) = default;

	GLPipelineObject::~GLPipelineObject() = default;

	const std::shared_ptr<Shader>& GLPipelineObject::getShader() const{
		return shader;
	}

	const VertexLayout& GLPipelineObject::getVertexLayout() const{
		return shaderReflectionData.vertexBufferLayout;
	}
}