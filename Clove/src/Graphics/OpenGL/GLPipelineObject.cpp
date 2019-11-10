#include "GLPipelineObject.hpp"

#include "Graphics/OpenGL/GLShader.hpp"

namespace clv::gfx::ogl{
	GLPipelineObject::GLPipelineObject(const std::shared_ptr<Shader>& shader)
		: shader(shader){
		const auto glShader = std::static_pointer_cast<GLShader>(shader);

		shaderReflectionData = shader->getReflectionData();
		const auto& layout = shaderReflectionData.vertexBufferLayout;

		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		uint64 offset = 0; //void* expects 64 bits
		for(uint32 i = 0; i < layout.count(); ++i){
			const auto& element = layout.resolve(i);
			const VertexElementType elementType = element.getType();

			GLint attribLoc = glGetAttribLocation(glShader->getProgramID(), element.getSemantic());

			glEnableVertexAttribArray(attribLoc);
			glVertexAttribPointer(attribLoc, element.getCount(), getGLElementType(elementType), isTypeNormalised(elementType), static_cast<GLsizei>(layout.size()), reinterpret_cast<const void*>(offset));
			offset += VertexElement::sizeOf(element.getType());
		}

		glBindVertexArray(0);
	}

	GLPipelineObject::GLPipelineObject(GLPipelineObject&& other) = default;

	GLPipelineObject& GLPipelineObject::operator=(GLPipelineObject&& other) = default;

	GLPipelineObject::~GLPipelineObject() = default;

	GLuint GLPipelineObject::getGLVertexArrayID() const{
		return vertexArrayID;
	}

	const std::shared_ptr<Shader>& GLPipelineObject::getShader() const{
		return shader;
	}

	const VertexLayout& GLPipelineObject::getVertexLayout() const{
		return shaderReflectionData.vertexBufferLayout;
	}

	GLenum GLPipelineObject::getGLElementType(VertexElementType type){
		switch(type){
			case VertexElementType::position2D:
			case VertexElementType::position3D:
			case VertexElementType::texture2D:
			case VertexElementType::normal:
				return GL_FLOAT;
			default:
				CLV_ASSERT(false, "Invalid element type");
				return GL_FLOAT;
		}
	}

	GLboolean GLPipelineObject::isTypeNormalised(VertexElementType type){
		switch(type){
			case VertexElementType::position2D:
			case VertexElementType::position3D:
			case VertexElementType::texture2D:
			case VertexElementType::normal:
				return GL_FALSE;
			default:
				CLV_ASSERT(false, "Invalid element type");
				return GL_FALSE;
		}
	}
}