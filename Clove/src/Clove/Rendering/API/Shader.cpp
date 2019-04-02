#include "clvpch.h"
#include "Shader.h"

#include <fstream>
#include <string>
#include <sstream>

#include "Clove/Rendering/Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace clv{
	Shader::Shader(const std::string& filepath)
		: filepath(filepath){
		ShaderProgramSource source = parseShader(filepath);
		rendererID = createShader(source.vertexSource, source.fragmentSource);
	}

	Shader::Shader(Shader&& other){
		filepath = other.filepath;
		rendererID = other.rendererID;
		uniformLocationCache = other.uniformLocationCache;

		other.filepath.clear();
		other.rendererID = 0;
		other.uniformLocationCache.clear();
	}

	Shader::~Shader(){
		GLCall(glDeleteProgram(rendererID));
	}

	void Shader::bind() const{
		GLCall(glUseProgram(rendererID));
	}

	void Shader::unbind() const{
		GLCall(glUseProgram(0));
	}

	void Shader::setUniform1i(const std::string& name, int value){
		GLCall(glUniform1i(getUniformLocation(name), value));
	}

	void Shader::setUniform1f(const std::string& name, float value){
		GLCall(glUniform1f(getUniformLocation(name), value));
	}

	void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2){
		GLCall(glUniform3f(getUniformLocation(name), v0, v1, v2));
	}

	void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3){
		GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
	}

	void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix){
		GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)));
	}

	Shader& Shader::operator=(Shader&& other){
		filepath = other.filepath;
		rendererID = other.rendererID;
		uniformLocationCache = other.uniformLocationCache;

		other.filepath.clear();
		other.rendererID = 0;
		other.uniformLocationCache.clear();

		return *this;
	}

	ShaderProgramSource Shader::parseShader(const std::string& filepath){
		std::ifstream stream(filepath);

		enum class shaderType{
			none = -1,
			vertex = 0,
			fragment = 1
		};

		std::string line;
		std::stringstream ss[2];
		shaderType type = shaderType::none;
		while(getline(stream, line)){
			if(line.find("#shader") != std::string::npos){
				if(line.find("vertex") != std::string::npos){
					type = shaderType::vertex;
				} else if(line.find("fragment") != std::string::npos){
					type = shaderType::fragment;
				}
			} else{
				ss[static_cast<int>(type)] << line << '\n';
			}
		}

		return { ss[0].str(), ss[1].str() };
	}

	unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader){
		GLCall(unsigned int program = glCreateProgram());
		unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

		GLCall(glAttachShader(program, vs));
		GLCall(glAttachShader(program, fs));
		GLCall(glLinkProgram(program));
		GLCall(glValidateProgram(program));

		GLCall(glDeleteShader(vs));
		GLCall(glDeleteShader(fs));

		return program;
	}	

	unsigned int Shader::compileShader(unsigned int type, const std::string& source){
		GLCall(unsigned int id = glCreateShader(type));
		const char* src = source.c_str();
		GLCall(glShaderSource(id, 1, &src, nullptr));
		GLCall(glCompileShader(id));

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if(result == GL_FALSE){
			int length;
			GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
			char* message = new char[length];
			GLCall(glGetShaderInfoLog(id, length, &length, message));
			CLV_ERROR("Failed to compile {0} shader! {1}", type == GL_VERTEX_SHADER ? "vertex" : "fragment", message);
			GLCall(glDeleteShader(id));
			delete[] message;
		}

		return id;
	}

	int Shader::getUniformLocation(const std::string& name){
		if(uniformLocationCache.find(name) != uniformLocationCache.end()){
			return uniformLocationCache[name];
		}

		GLCall(int location = glGetUniformLocation(rendererID, name.c_str()));
		if(location == -1){
			CLV_WARN("Warning: Uniform {0} does not exist!: {1}", name, __FUNCTION__);
		} 

		uniformLocationCache[name] = location;
		return location;
	}
}