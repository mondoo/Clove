#include "clvpch.hpp"
#include "Shader.hpp"

#include <fstream>
#include <string>
#include <sstream>

namespace clv{
	Shader::Shader(){
		rendererID = createShader();
	}

	Shader::Shader(Shader&& other){
		rendererID = other.rendererID;
		uniformLocationCache = std::move(other.uniformLocationCache);

		other.rendererID = 0;
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

	void Shader::attachShader(ShaderTypes shaderType, const std::string& path){
		//TODO: move to function when other APIs are in
		unsigned int type = 0;
		switch(shaderType){
		case ShaderTypes::Vertex:
			type = GL_VERTEX_SHADER;
			break;
		case ShaderTypes::Fragment:
			type = GL_FRAGMENT_SHADER;
			break;
		}

		CLV_ASSERT(type != 0, "Shader type not set!");

		std::string source = parseShader(path);

		unsigned int id = compileShader(type, source);

		GLCall(glAttachShader(rendererID, id));
		GLCall(glLinkProgram(rendererID));
		GLCall(glValidateProgram(rendererID));
		GLCall(glDeleteShader(id));
	}

	Shader& Shader::operator=(Shader&& other){
		rendererID = other.rendererID;
		uniformLocationCache = std::move(other.uniformLocationCache);

		other.rendererID = 0;

		return *this;
	}

	std::string Shader::parseShader(const std::string& filepath){
		std::ifstream stream(filepath);

		std::string line;
		std::stringstream ss;
		while(getline(stream, line)){
			ss << line << '\n';
		}

		return ss.str();
	}

	unsigned int Shader::createShader(){
		GLCall(unsigned int program = glCreateProgram());
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