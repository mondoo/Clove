#pragma once

#include "Clove/Graphics/Core/PipelineObject.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	class GLPipelineObject : public PipelineObject{
		//VARIABLES
	private:
		GLuint vertexArrayID = 0;

		std::shared_ptr<Shader> shader;
		ShaderReflectionData shaderReflectionData;

		//FUNCTIONS
	public:
		GLPipelineObject() = delete;
		GLPipelineObject(const std::shared_ptr<Shader>& shader);
		GLPipelineObject(const GLPipelineObject& other) = delete;
		GLPipelineObject(GLPipelineObject&& other);
		GLPipelineObject& operator=(const GLPipelineObject& other) = delete;
		GLPipelineObject& operator=(GLPipelineObject&& other);
		virtual ~GLPipelineObject();

		GLuint getGLVertexArrayID() const;

		virtual const std::shared_ptr<Shader>& getShader() const override;
		virtual const VertexLayout& getVertexLayout() const override;

	private:
		GLenum getGLElementType(VertexElementType type);
		GLboolean isTypeNormalised(VertexElementType type);
	};
}