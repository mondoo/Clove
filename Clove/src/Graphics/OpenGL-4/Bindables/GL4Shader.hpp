#pragma once

#include "Core/Graphics/Bindables/Shader.hpp"

#include <glad/glad.h>

namespace clv::gfx{
	class Renderer;

	class GL4Shader : public Shader{
		//VARIABLES
	private:
		uint32 programID = 0;

		//FUNCTIONS
	public:
		GL4Shader() = delete;
		GL4Shader(const GL4Shader& other) = delete;
		GL4Shader(GL4Shader&& other) noexcept;
		GL4Shader& operator=(const GL4Shader& other) = delete;
		GL4Shader& operator=(GL4Shader&& other) noexcept;
		~GL4Shader();

		GL4Shader(ShaderStyle style);

		virtual void bind() override;

		virtual ShaderReflectionData getReflectionData() override;

		uint32 getProgramID() const;

	private:
		void initialise(ShaderStyle style);

		uint32 compileShader(uint32 type, const std::string& source);

		void attachAndLinkShader(uint32 shaderID);

		std::string getStringFromShaderType(GLuint glShaderType);
	};
}