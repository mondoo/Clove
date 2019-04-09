#pragma once

#include "Clove/Rendering/Renderer.hpp" //TODO: this is to get the GLCall macro

#include <glad/glad.h>

namespace clv{
	enum class ShaderTypes{
		Vertex,
		Fragment,
	};

	class Shader{
		//VARIABLES
	private:
		unsigned int rendererID = 0;
		std::unordered_map<std::string, int> uniformLocationCache;

		//FUNCTIONS
	public:
		Shader();
		Shader(Shader&& other) noexcept;

		~Shader();

		void bind() const;
		void unbind() const;

		void attachShader(ShaderTypes shaderType, const std::string& path);

		template<typename T>
		void setUniform(const std::string& name, const T& value);

		template<>
		void setUniform<int>(const std::string& name, const int& value);
		template<>
		void setUniform<float>(const std::string& name, const float& value);
		template<>
		void setUniform<math::Vector3f>(const std::string& name, const math::Vector3f& value);
		template<>
		void setUniform<math::Vector4f>(const std::string& name, const math::Vector4f& value);
		template<>
		void setUniform<math::Matrix4f>(const std::string& name, const math::Matrix4f& value);

		Shader& operator=(Shader&& other) noexcept;

	private:
		std::string parseShader(const std::string& filepath);
		unsigned int createShader();
		unsigned int compileShader(unsigned int type, const std::string& source);

		int getUniformLocation(const std::string& name);
	};
}

#include "Shader.inl"