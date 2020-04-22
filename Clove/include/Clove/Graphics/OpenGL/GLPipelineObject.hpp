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

		~GLPipelineObject();

		const std::shared_ptr<GraphicsFactory>& getFactory() const override;

		void setVertexShader(const Shader& vertexShader) override;
		void setGeometryShader(const Shader& geometryShader) override;
		void setPixelShader(const Shader& pixelShader) override;

		void setBlendState(bool enabled) override;
		void setCullMode(CullFace face, bool frontFaceCounterClockwise) override;

		const VertexLayout& getVertexLayout() const override;

		GLuint getGLVertexArrayID() const;
		GLuint getGLPorgramID() const;

		bool isBlendEnabled() const;
		CullFace getCullFace() const;
		bool isFrontFaceCounterClockwise() const;

	private:
		void attachAndLinkShader(GLuint shaderID);
	};
}