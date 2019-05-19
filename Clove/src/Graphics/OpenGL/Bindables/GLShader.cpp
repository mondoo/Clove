#include "clvpch.hpp"
#include "GLShader.hpp"

#include "Graphics/OpenGL/GLException.hpp"

#include <glad/glad.h>
#include <fstream>
#include <sstream>

namespace clv::gfx{
	GLShader::GLShader(){
		GLCall(programID = glCreateProgram());
	}

	GLShader::GLShader(GLShader&& other) noexcept{
		programID = other.programID;
		other.programID = 0;

		uniformLocationCache = other.uniformLocationCache;
	}

	GLShader& GLShader::operator=(GLShader&& other) noexcept{
		programID = other.programID;
		other.programID = 0;

		uniformLocationCache = other.uniformLocationCache;

		return *this;
	}

	GLShader::~GLShader(){
		GLCall(glDeleteProgram(programID));
	}

	void GLShader::bind(Renderer& renderer){
		GLCall(glUseProgram(programID));
		isBound = true;

		for(const auto&[uniformName, value] : mvpMap){
			GLCall(glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, math::valuePtr(value)));
		}
	}

	void GLShader::unbind(){
		GLCall(glUseProgram(0));
		isBound = false;
	}

	void GLShader::attachShader(ShaderTypes type, Renderer& renderer){
		unsigned int gltype = 0;
		switch(type){
			case ShaderTypes::Vertex:
				gltype = GL_VERTEX_SHADER;
				break;
			case ShaderTypes::Pixel:
				gltype = GL_FRAGMENT_SHADER;
				break;
		}

		CLV_ASSERT(gltype != 0, "Shader type not set!");

		std::string shaderSource = parseShader(getPathForShader(type));
		unsigned int shaderID = compileShader(gltype, shaderSource);

		GLCall(glAttachShader(programID, shaderID));
		GLCall(glLinkProgram(programID));
		GLCall(glValidateProgram(programID));
		GLCall(glDeleteShader(shaderID));
	}

	void GLShader::setModelMatrix(const math::Matrix4f& model){
		if(isBound){
			GLCall(glUniformMatrix4fv(getUniformLocation("model"), 1, GL_FALSE, math::valuePtr(model)));
		}
		mvpMap["model"] = model;
	}

	void GLShader::setViewMatrix(const math::Matrix4f& view){
		if(isBound){
			GLCall(glUniformMatrix4fv(getUniformLocation("view"), 1, GL_FALSE, math::valuePtr(view)));
		}
		mvpMap["view"] = view;

	}

	void GLShader::setProjectionMatrix(const math::Matrix4f& projection){
		if(isBound){
			GLCall(glUniformMatrix4fv(getUniformLocation("projection"), 1, GL_FALSE, math::valuePtr(projection)));
		}
		mvpMap["projection"] = projection;
	}

	std::string GLShader::getPathForShader(ShaderTypes shader){
		switch(shader){
			case ShaderTypes::Vertex:
				return "../Clove/src/Graphics/OpenGL/Shaders/Default-vs.glsl";
				break;

			case ShaderTypes::Pixel:
				return "../Clove/src/Graphics/OpenGL/Shaders/Default-ps.glsl";
				break;

			default:
				CLV_ASSERT(false, "Unknown type! " __FUNCTION__);
				return std::string();
				break;
		}
	}

	std::string GLShader::parseShader(const std::string& filepath){
		std::ifstream stream(filepath);

		std::string line;
		std::stringstream ss;
		while(getline(stream, line)){
			ss << line << '\n';
		}

		return ss.str();
	}

	unsigned int GLShader::compileShader(unsigned int type, const std::string& source){
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
			CLV_LOG_ERROR("Failed to compile {0} shader! {1}", type == GL_VERTEX_SHADER ? "vertex" : "fragment", message);
			GLCall(glDeleteShader(id));
			delete[] message;
		}

		return id;
	}

	int GLShader::getUniformLocation(const std::string& name){
		if(uniformLocationCache.find(name) != uniformLocationCache.end()){
			return uniformLocationCache[name];
		}

		GLCall(int location = glGetUniformLocation(programID, name.c_str()));
		if(location == -1){
			CLV_LOG_WARN("Warning: Uniform {0} does not exist!: {1}", name, __FUNCTION__);
		}

		uniformLocationCache[name] = location;
		return location;
	}
}