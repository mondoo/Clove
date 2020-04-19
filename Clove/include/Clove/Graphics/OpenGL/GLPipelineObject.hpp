#pragma once

#include "Clove/Graphics/PipelineObject.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	class GLPipelineObject : public PipelineObject{
		//VARIABLES
	private:
		std::shared_ptr<GraphicsFactory> factory;

		VertexLayout vertexLayout;

		bool blendEnabled = false;
		CullFace cullFace = CullFace::Back;
		bool frontFaceCounterClockwise = true;

		GLuint vertexArrayID = 0;
		GLuint programID = 0;

		//FUNCTIONS
	public:
		GLPipelineObject(std::shared_ptr<GraphicsFactory> factory);

		GLPipelineObject(const GLPipelineObject& other) = delete;
		GLPipelineObject(GLPipelineObject&& other) noexcept;

		GLPipelineObject& operator=(const GLPipelineObject& other) = delete;
		GLPipelineObject& operator=(GLPipelineObject&& other) noexcept;

		virtual ~GLPipelineObject();

		virtual const std::shared_ptr<GraphicsFactory>& getFactory() const override;

		virtual void setVertexShader(const Shader& vertexShader) override;
		virtual void setGeometryShader(const Shader& geometryShader) override;
		virtual void setPixelShader(const Shader& pixelShader) override;

		virtual void setBlendState(bool enabled) override;
		virtual void setCullMode(CullFace face, bool frontFaceCounterClockwise) override;

		virtual const VertexLayout& getVertexLayout() const override;

		GLuint getGLVertexArrayID() const;
		GLuint getGLPorgramID() const;

		bool isBlendEnabled() const;
		CullFace getCullFace() const;
		bool isFrontFaceCounterClockwise() const;

	private:
		void attachAndLinkShader(GLuint shaderID);
	};
}