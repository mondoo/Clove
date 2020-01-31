#pragma once

#include "Clove/Graphics/Core/Shader.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	class Renderer;

	class GLShader : public Shader{
		//VARIABLES
	private:
		uint32 shaderID = 0;

		ShaderDescriptor descriptor;

		//FUNCTIONS
	public:
		GLShader() = delete;
		GLShader(const ShaderDescriptor& descriptor, std::string_view pathToShader);

		GLShader(const GLShader& other) = delete;
		GLShader(GLShader&& other) noexcept;

		GLShader& operator=(const GLShader& other) = delete;
		GLShader& operator=(GLShader&& other) noexcept;

		virtual ~GLShader();

		virtual const ShaderDescriptor& getDescriptor() const override;

		uint32 getShaderID() const;

	private:
		void compileShader(GLenum stage, std::string_view source);

		GLenum getGLShaderStage(const ShaderStage stage);
		std::string getStringFromShaderStage(GLuint glShaderStage);
	};
}