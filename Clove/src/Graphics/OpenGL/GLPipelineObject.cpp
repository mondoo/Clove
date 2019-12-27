#include "Clove/Graphics/OpenGL/GLPipelineObject.hpp"

#include "Clove/Graphics/OpenGL/GLShader.hpp"

namespace clv::gfx::ogl{
	GLPipelineObject::GLPipelineObject(const std::shared_ptr<Shader>& shader)
		: shader(shader){
		const auto glShader = std::static_pointer_cast<GLShader>(shader);

		shaderReflectionData = glShader->getReflectionData();
		const auto& layout = shaderReflectionData.vertexBufferLayout;

		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		GLuint offset = 0;
		for(uint32 i = 0; i < layout.count(); ++i){
			const auto& element = layout.resolve(i);
			const VertexElementType elementType = element.getType();

			GLint attribLoc = glGetAttribLocation(glShader->getProgramID(), element.getSemantic());

			glEnableVertexAttribArray(attribLoc);
			glVertexAttribFormat(attribLoc, element.getCount(), getGLElementType(elementType), isTypeNormalised(elementType), offset);
			glVertexAttribBinding(attribLoc, 0);
			
			offset += VertexElement::sizeOf(elementType);
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
