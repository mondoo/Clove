#include "Clove/Graphics/OpenGL/GLPipelineObject.hpp"

#include "Clove/Graphics/OpenGL/GLShader.hpp"

namespace clv::gfx::ogl {
	static GLenum getGLElementType(VertexElementType type) {
		switch(type) {
			case VertexElementType::position2D:
			case VertexElementType::position3D:
			case VertexElementType::texture2D:
			case VertexElementType::normal:
			case VertexElementType::colour3D:
				return GL_FLOAT;
			default:
				CLV_ASSERT(false, "Invalid element type");
				return GL_FLOAT;
		}
	}

	static GLboolean isTypeNormalised(VertexElementType type) {
		switch(type) {
			case VertexElementType::position2D:
			case VertexElementType::position3D:
			case VertexElementType::texture2D:
			case VertexElementType::normal:
			case VertexElementType::colour3D:
				return GL_FALSE;
			default:
				CLV_ASSERT(false, "Invalid element type");
				return GL_FALSE;
		}
	}

	GLPipelineObject::GLPipelineObject(std::shared_ptr<GraphicsFactory> factory)
		: factory(std::move(factory))
		, programID(glCreateProgram()) {
		setBlendState(true);
		setCullMode(CullFace::Back, true);
	}

	GLPipelineObject::GLPipelineObject(GLPipelineObject&& other) noexcept = default;

	GLPipelineObject& GLPipelineObject::operator=(GLPipelineObject&& other) noexcept = default;

	GLPipelineObject::~GLPipelineObject() = default;

	const std::shared_ptr<GraphicsFactory>& GLPipelineObject::getFactory() const {
		return factory;
	}

	void GLPipelineObject::setVertexShader(const Shader& vertexShader) {
		const GLShader& glShader = static_cast<const GLShader&>(vertexShader);
		attachAndLinkShader(glShader.getShaderID());

		vertexLayout.clear();
		GLint attribCount = 0;
		glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTES, &attribCount);
		for(int32_t i = 0; i < attribCount; ++i) {
			GLchar name[255];
			GLsizei length = 0;
			GLint size = 0;
			GLenum type = 0;

			glGetActiveAttrib(programID, static_cast<GLuint>(i), sizeof(name), &length, &size, &type, name);

			vertexLayout.add(VertexElement::getTypeFromSemantic(name));
		}

		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		GLuint offset = 0;
		for(uint32_t i = 0; i < vertexLayout.count(); ++i) {
			const auto& element = vertexLayout.resolve(i);
			const VertexElementType elementType = element.getType();

			GLint attribLoc = glGetAttribLocation(programID, element.getSemantic().data());

			glEnableVertexAttribArray(attribLoc);
			glVertexAttribFormat(attribLoc, element.getCount(), getGLElementType(elementType), isTypeNormalised(elementType), offset);
			glVertexAttribBinding(attribLoc, 0);

			offset += VertexElement::sizeOf(elementType);
		}

		glBindVertexArray(0);
	}

	void GLPipelineObject::setGeometryShader(const Shader& geometryShader) {
		const GLShader& glShader = static_cast<const GLShader&>(geometryShader);
		attachAndLinkShader(glShader.getShaderID());
	}

	void GLPipelineObject::setPixelShader(const Shader& pixelShader) {
		const GLShader& glShader = static_cast<const GLShader&>(pixelShader);
		attachAndLinkShader(glShader.getShaderID());
	}

	void GLPipelineObject::setBlendState(bool enabled) {
		blendEnabled = enabled;
	}

	void GLPipelineObject::setCullMode(CullFace face, bool frontFaceCounterClockwise) {
		cullFace = face;
		this->frontFaceCounterClockwise = frontFaceCounterClockwise;
	}

	const VertexLayout& GLPipelineObject::getVertexLayout() const {
		return vertexLayout;
	}

	GLuint GLPipelineObject::getGLVertexArrayID() const {
		return vertexArrayID;
	}

	GLuint GLPipelineObject::getGLPorgramID() const {
		return programID;
	}

	bool GLPipelineObject::isBlendEnabled() const {
		return blendEnabled;
	}

	CullFace GLPipelineObject::getCullFace() const {
		return cullFace;
	}

	bool GLPipelineObject::isFrontFaceCounterClockwise() const {
		return frontFaceCounterClockwise;
	}

	void GLPipelineObject::attachAndLinkShader(GLuint shaderID) {
		glAttachShader(programID, shaderID);
		glLinkProgram(programID);
		glValidateProgram(programID);
	}
}
