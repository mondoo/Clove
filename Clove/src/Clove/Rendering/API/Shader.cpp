#include "clvpch.hpp"
#include "Shader.hpp"

#include "Clove/Rendering/API/GLHelpers.hpp"

#include <glad/glad.h>

#include <fstream>
#include <string>
#include <sstream>

namespace clv{
	Shader::Shader(){
		rendererID = createShader();
	}

	Shader::Shader(Shader&& other) noexcept{
		rendererID = other.rendererID;
		uniformLocationCache = std::move(other.uniformLocationCache);
		attachedShaders = std::move(other.attachedShaders);

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
		
		attachedShaders[shaderType] = parseShader(path);

		insertShaderDefines();
	}

	void Shader::setNumberOfDirectionalLights(int num){
		numOfDirectionalLights = num;
		insertShaderDefines();
	}

	void Shader::setNumberOfPointLights(int num){
		numOfPointLights = num;
		insertShaderDefines();
	}

	void Shader::setUniform(const std::string& name, const int& value){
		GLCall(glUniform1i(getUniformLocation(name), value));
	}

	void Shader::setUniform(const std::string& name, const float& value){
		GLCall(glUniform1f(getUniformLocation(name), value));
	}

	void Shader::setUniform(const std::string& name, const math::Vector3f& value){
		GLCall(glUniform3f(getUniformLocation(name), value.x, value.y, value.z));
	}

	void Shader::setUniform(const std::string& name, const math::Vector4f& value){
		GLCall(glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w));
	}

	void Shader::setUniform(const std::string& name, const math::Matrix4f& value){
		GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, math::valuePtr(value)));
	}

	Shader& Shader::operator=(Shader&& other) noexcept{
		rendererID = other.rendererID;
		uniformLocationCache = std::move(other.uniformLocationCache);
		attachedShaders = std::move(other.attachedShaders);

		other.rendererID = 0;

		return *this;
	}

	unsigned int Shader::createShader(){
		GLCall(unsigned int program = glCreateProgram());
		return program;
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

	void Shader::insertShaderDefines(){
		if(attachedShaders.find(ShaderTypes::Fragment) != attachedShaders.end()){
			CLV_WARN(__FUNCTION__ " called, tearing down shader and recompiling");

			std::string defaultShaderSource = attachedShaders[ShaderTypes::Fragment];
			
			unbind();

			int newRendererID = createShader();
			GLCall(glDeleteProgram(rendererID));
			rendererID = newRendererID;

			std::string::iterator sourceIterator = defaultShaderSource.begin() + defaultShaderSource.find("\n") + 2;

			CLV_ASSERT(sourceIterator != defaultShaderSource.end(), "Reached end of file before finding new line segment");

			if(numOfDirectionalLights != 0){
				const std::string seq = "#define NUM_DIR_LIGHTS " + std::to_string(numOfDirectionalLights) + "\n";
				sourceIterator = defaultShaderSource.insert(sourceIterator, seq.begin(), seq.end());
			}

			if(numOfPointLights != 0){
				const std::string seq = "#define NUM_POINT_LIGHTS " + std::to_string(numOfPointLights) + "\n";
				sourceIterator = defaultShaderSource.insert(sourceIterator, seq.begin(), seq.end());
			}

			unsigned int id = compileShader(GL_FRAGMENT_SHADER, defaultShaderSource);

			linkShader(id);

			if(attachedShaders.find(ShaderTypes::Vertex) != attachedShaders.end()){
				unsigned int id = compileShader(GL_VERTEX_SHADER, attachedShaders[ShaderTypes::Vertex]);
				linkShader(id);
			}

			uniformLocationCache.clear();

			bind();
		}
	}

	void Shader::linkShader(unsigned int shaderID){
		GLCall(glAttachShader(rendererID, shaderID));
		GLCall(glLinkProgram(rendererID));
		GLCall(glValidateProgram(rendererID));
		GLCall(glDeleteShader(shaderID));
	}
}