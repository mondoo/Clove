#pragma once

namespace clv{
	struct ShaderProgramSource{
		std::string vertexSource;
		std::string fragmentSource;
	};

	class CLV_API Shader{
		//VARIABLES
	private:
		std::string filepath;
		unsigned int rendererID = 0;
		std::unordered_map<std::string, int> uniformLocationCache;
		//FUNCTIONS
	public:
		Shader() = default; //temp
		Shader(const std::string& filepath);
		~Shader();

		void bind() const;
		void unbind() const;

		void deleteShader();

		//TODO make this set value / set unform templated function
		void setUniform1i(const std::string& name, int value);
		void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

	private:
		ShaderProgramSource parseShader(const std::string& filepath);
		unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
		unsigned int compileShader(unsigned int type, const std::string& source);

		int getUniformLocation(const std::string& name);
	};
}
