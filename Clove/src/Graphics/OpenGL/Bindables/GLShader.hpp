#pragma once

#include "Clove/Graphics/Bindables/Shader.hpp"

namespace clv::gfx{
	class Renderer;

	class GLShader : public Shader{
		//VARIABLES
	private:
		unsigned int programID = 0;
		std::unordered_map<std::string, int> uniformLocationCache;

		std::unordered_map<std::string, math::Matrix4f> mvpMap;

		bool isBound = false;

		//FUNCTIONS
	public:
		GLShader();
		GLShader(const GLShader& other) = delete;
		GLShader(GLShader&& other) noexcept;
		GLShader& operator=(const GLShader& other) = delete;
		GLShader& operator=(GLShader&& other) noexcept;
		~GLShader();

		virtual void bind(Renderer& renderer) override;
		virtual void unbind() override;

		virtual void attachShader(ShaderTypes type) override;

		virtual void setWorldMatrix(const math::Matrix4f& world) override;
		virtual void setViewMatrix(const math::Matrix4f& view) override;
		virtual void setProjectionMatrix(const math::Matrix4f& projection) override;

	private:
		std::string getPathForShader(ShaderTypes shader);

		std::string parseShader(const std::string& filepath);
		unsigned int compileShader(unsigned int type, const std::string& source);

		int getUniformLocation(const std::string& name);
	};
}