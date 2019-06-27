#pragma once

#include "Clove/Graphics/Bindables/Shader.hpp"

#include "Graphics/OpenGL-4/GL4Uniform.hpp"

namespace clv::gfx{
	class Renderer;

	class GL4Shader : public Shader{
		//VARIABLES
	private:
		unsigned int programID = 0;

		GL4Uniform<math::Matrix4f> modelUniform;
		GL4Uniform<math::Matrix4f> normalMatrixUniform;
		GL4Uniform<int> diffuseSlotUniform;
		GL4Uniform<int> specularSlotUniform;
		GL4Uniform<float> matShininess;

		//FUNCTIONS
	public:
		GL4Shader();
		GL4Shader(const GL4Shader& other) = delete;
		GL4Shader(GL4Shader&& other) noexcept = default;
		GL4Shader& operator=(const GL4Shader& other) = delete;
		GL4Shader& operator=(GL4Shader&& other) noexcept = default;
		~GL4Shader();

		virtual void bind(Renderer& renderer) override;
		virtual void unbind() override;

		virtual void attachShader(ShaderTypes type) override;

		virtual void setModelMatrix(const math::Matrix4f& model) override;

	private:
		std::string getPathForShader(ShaderTypes shader);

		std::string parseShader(const std::string& filepath);
		unsigned int compileShader(unsigned int type, const std::string& source);
	};
}