#pragma once

#include "Clove/Graphics/Core/PipelineObject.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	class GLPipelineObject : public PipelineObject{
		//VARIABLES
	private:
		std::shared_ptr<Shader> shader;
		ShaderReflectionData shaderReflectionData;

		bool blendEnabled = false;
		CullFace cullFace = CullFace::Back;
		bool frontFaceCounterClockwise = true;

		GLuint vertexArrayID = 0;

		//FUNCTIONS
	public:
		GLPipelineObject() = delete;
		GLPipelineObject(const std::shared_ptr<Shader>& shader);

		GLPipelineObject(const GLPipelineObject& other) = delete;
		GLPipelineObject(GLPipelineObject&& other);

		GLPipelineObject& operator=(const GLPipelineObject& other) = delete;
		GLPipelineObject& operator=(GLPipelineObject&& other);

		virtual ~GLPipelineObject();

		virtual void setBlendState(bool enabled) override;
		virtual void setCullMode(CullFace face, bool frontFaceCounterClockwise) override;

		virtual const std::shared_ptr<Shader>& getShader() const override;
		virtual const VertexLayout& getVertexLayout() const override;

		GLuint getGLVertexArrayID() const;
		bool isBlendEnabled() const;
		CullFace getCullFace() const;
		bool isFrontFaceCounterClockwise() const;

	private:
		GLenum getGLElementType(VertexElementType type);
		GLboolean isTypeNormalised(VertexElementType type);
	};
}