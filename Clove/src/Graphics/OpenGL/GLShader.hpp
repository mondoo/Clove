#pragma once

#include "Core/Graphics/Shader.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	class Renderer;

	class GLShader : public Shader{
		//VARIABLES
	private:
		uint32 programID = 0;

		ShaderDescriptor descriptor;

		//FUNCTIONS
	public:
		GLShader() = delete;
		GLShader(const ShaderDescriptor& descriptor);
		GLShader(const GLShader& other) = delete;
		GLShader(GLShader&& other) noexcept;
		GLShader& operator=(const GLShader& other) = delete;
		GLShader& operator=(GLShader&& other) noexcept;
		~GLShader();

		uint32 getProgramID() const;

		virtual const ShaderDescriptor& getDescriptor() const override;
		virtual ShaderReflectionData getReflectionData() const override;

	private:
		void initialise(ShaderStyle style);

		uint32 compileShader(uint32 type, const std::string& source);

		void attachAndLinkShader(uint32 shaderID);

		std::string getStringFromShaderType(GLuint glShaderType);
	};
}