#pragma once

#include "Clove/Rendering/Renderer.h" //TODO: this is to get the GLCall macro
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
		Shader(Shader&& other);

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
		void setUniform<glm::vec3>(const std::string& name, const glm::vec3& value);
		template<>
		void setUniform<glm::vec4>(const std::string& name, const glm::vec4& value);
		template<>
		void setUniform<glm::mat4>(const std::string& name, const glm::mat4& value);

		Shader& operator=(Shader&& other);

	private:
		std::string parseShader(const std::string& filepath);
		unsigned int createShader();
		unsigned int compileShader(unsigned int type, const std::string& source);

		int getUniformLocation(const std::string& name);
	};
}

#include "Shader.inl"