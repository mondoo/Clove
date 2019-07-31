#include "clvpch.hpp"
#include "GL4Shader.hpp"

#include "Clove/Application.hpp"

#include <glad/glad.h>
#include <fstream>
#include <sstream>

namespace clv::gfx{
	GL4Shader::GL4Shader()
		: programID(glCreateProgram()){
	}

	GL4Shader::GL4Shader(GL4Shader&& other) noexcept = default;

	GL4Shader& GL4Shader::operator=(GL4Shader&& other) noexcept = default;

	GL4Shader::~GL4Shader(){
		glDeleteProgram(programID);
	}

	void GL4Shader::bind(){
		glUseProgram(programID);
	}

	void GL4Shader::attachShader(ShaderType type){
		unsigned int gltype = 0;
		switch(type){
			case ShaderType::Vertex:
			case ShaderType::Vertex2D:
			case ShaderType::VertexFB:
				gltype = GL_VERTEX_SHADER;
				break;
			case ShaderType::Pixel:
			case ShaderType::Pixel2D:
			case ShaderType::PixelFB:
				gltype = GL_FRAGMENT_SHADER;
				break;
		}

		CLV_ASSERT(gltype != 0, "Shader type not set!");

		std::string shaderSource = parseShader(getPathForShader(type));
		unsigned int shaderID = compileShader(gltype, shaderSource);

		glAttachShader(programID, shaderID);
		glLinkProgram(programID);
		glValidateProgram(programID);
		glDeleteShader(shaderID);

		//Would have something here just to set up the points on the textures depending on shader type
	}

	std::string GL4Shader::getPathForShader(ShaderType shader){
		switch(shader){
			case ShaderType::Vertex:
				return "../Clove/src/Graphics/OpenGL-4/Shaders/Default-vs.glsl";
				break;

			case ShaderType::Pixel:
				return "../Clove/src/Graphics/OpenGL-4/Shaders/Default-ps.glsl";
				break;

			case ShaderType::Vertex2D:
				return "../Clove/src/Graphics/OpenGL-4/Shaders/2D-vs.glsl";
				break;

			case ShaderType::Pixel2D:
				return "../Clove/src/Graphics/OpenGL-4/Shaders/2D-ps.glsl";
				break;

			case ShaderType::VertexFB:
				return "../Clove/src/Graphics/OpenGL-4/Shaders/FrameBuffer-vs.glsl";
				break;

			case ShaderType::PixelFB:
				return "../Clove/src/Graphics/OpenGL-4/Shaders/FrameBuffer-ps.glsl";
				break;

			default:
				CLV_ASSERT(false, "Unknown type! {0}", __func__);
				return std::string();
				break;
		}
	}

	std::string GL4Shader::parseShader(const std::string& filepath){
		std::ifstream stream(filepath);

		std::string line;
		std::stringstream ss;
		while(getline(stream, line)){
			ss << line << '\n';
		}

		return ss.str();
	}

	unsigned int GL4Shader::compileShader(unsigned int type, const std::string& source){
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if(result == GL_FALSE){
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = new char[length];
			glGetShaderInfoLog(id, length, &length, message);
			CLV_LOG_ERROR("Failed to compile {0} shader! {1}", type == GL_VERTEX_SHADER ? "vertex" : "fragment", message);
			glDeleteShader(id);
			delete[] message;
		}

		return id;
	}
}