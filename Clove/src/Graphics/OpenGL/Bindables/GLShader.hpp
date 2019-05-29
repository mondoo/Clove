#pragma once

#include "Clove/Graphics/Bindables/Shader.hpp"

#include "Graphics/OpenGL/GLUniform.hpp"

namespace clv::gfx{
	class Renderer;

	class GLShader : public Shader{
		//VARIABLES
	private:
		unsigned int programID = 0;

		GLUniform<math::Matrix4f> modelUniform;
		GLUniform<math::Matrix4f> normalMatrixUniform;
		GLUniform<int> diffuseSlotUniform;
		GLUniform<int> specularSlotUniform;
		GLUniform<float> matShininess;

		//FUNCTIONS
	public:
		GLShader();
		GLShader(const GLShader& other) = delete;
		GLShader(GLShader&& other) noexcept = default;
		GLShader& operator=(const GLShader& other) = delete;
		GLShader& operator=(GLShader&& other) noexcept = default;
		~GLShader();

		virtual void bind(Renderer& renderer) override;
		virtual void unbind() override;

		virtual void attachShader(ShaderTypes type) override;

		virtual void setModelMatrix(const math::Matrix4f& world) override;

	private:
		std::string getPathForShader(ShaderTypes shader);

		std::string parseShader(const std::string& filepath);
		unsigned int compileShader(unsigned int type, const std::string& source);
	};
}