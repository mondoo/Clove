#pragma once

#include <glm/glm.hpp>

namespace clv{
	struct ShaderProgramSource{
		std::string vertexSource;
		std::string fragmentSource;
	};

	class Shader{
		//VARIABLES
	private:
		std::string filepath;
		unsigned int rendererID = 0;
		std::unordered_map<std::string, int> uniformLocationCache;

		//FUNCTIONS
	public:
		Shader(const std::string& filepath);
		Shader(Shader&& other);

		~Shader();

		void bind() const;
		void unbind() const;

		//TODO make this set value / set unform templated function
		void setUniform1i(const std::string& name, int value);
		void setUniform1f(const std::string& name, float value);
		void setUniform3f(const std::string& name, float v0, float v1, float v2);
		void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

		Shader& operator=(Shader&& other);

	private:
		ShaderProgramSource parseShader(const std::string& filepath);
		unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
		unsigned int compileShader(unsigned int type, const std::string& source);

		int getUniformLocation(const std::string& name);
	};
}
