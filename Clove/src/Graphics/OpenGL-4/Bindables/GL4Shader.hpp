#pragma once

#include "Clove/Graphics/Bindables/Shader.hpp"

#include "Graphics/OpenGL-4/Bindables/GL4UniformBufferObject.hpp"

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

	private:
		void initialise(ShaderStyle style);

		std::string parseShader(const std::string& filepath);
		uint32 compileShader(uint32 type, const std::string& source);
	};
}