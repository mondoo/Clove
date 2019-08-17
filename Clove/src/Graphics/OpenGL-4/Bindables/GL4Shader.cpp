#include "clvpch.hpp"
#include "GL4Shader.hpp"

#include "Clove/Application.hpp"

#include <glad/glad.h>
#include <fstream>
#include <sstream>

namespace clv::gfx{
	GL4Shader::GL4Shader(GL4Shader&& other) noexcept = default;

	GL4Shader& GL4Shader::operator=(GL4Shader&& other) noexcept = default;

	GL4Shader::~GL4Shader(){
		glDeleteProgram(programID);
	}

	GL4Shader::GL4Shader(ShaderStyle style)
		: programID(glCreateProgram()){
		initialise(style);
	}

	void GL4Shader::bind(){
		glUseProgram(programID);
	}

	ShaderReflectionData GL4Shader::getReflectionData(){
		GLint uniformCount = 0;
		GLint uniformBlockCount = 0;

		glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &uniformCount);
		glGetProgramiv(programID, GL_ACTIVE_UNIFORM_BLOCKS, &uniformBlockCount);

		CLV_LOG_DEBUG("SID:{0} | Active uniforms on shader: {1}", programID, uniformCount);
		CLV_LOG_DEBUG("SID:{0} | Active uniform blocks on shader: {1}", programID, uniformBlockCount);

		CLV_LOG_DEBUG("SID:{0} | Block binding points for current shader as follows:", programID);
		
		for(int32 i = 0; i < uniformBlockCount; ++i){
			GLint bindingPoint = 0;
			GLint size = 0;
			glGetActiveUniformBlockiv(programID, static_cast<GLuint>(i), GL_UNIFORM_BLOCK_BINDING, &bindingPoint);
			glGetActiveUniformBlockiv(programID, static_cast<GLuint>(i), GL_UNIFORM_BLOCK_DATA_SIZE, &size);

			CLV_LOG_DEBUG("	| binding point: {0}", bindingPoint);
			CLV_LOG_DEBUG("	| size: {0}", size);
		}

		return {}; //Empty boy for testing
	}

	void GL4Shader::initialise(ShaderStyle style){
		uint32 vertexID = 0;
		uint32 pixelID = 0;
		
		switch(style){
			case ShaderStyle::Lit:
				{
					std::string vertexSource = parseShader("Lit-vs.glsl");
					vertexID = compileShader(GL_VERTEX_SHADER, vertexSource);

					std::string pixelSource = parseShader("Lit-ps.glsl");
					pixelID = compileShader(GL_FRAGMENT_SHADER, pixelSource);
				}
				break;
			case ShaderStyle::Unlit:
				{
					std::string vertexSource = parseShader("Unlit-vs.glsl");
					vertexID = compileShader(GL_VERTEX_SHADER, vertexSource);

					std::string pixelSource = parseShader("Unlit-ps.glsl");
					pixelID = compileShader(GL_FRAGMENT_SHADER, pixelSource);
				}
				break;

			case ShaderStyle::_2D:
				{
					std::string vertexSource = parseShader("2D-vs.glsl");
					vertexID = compileShader(GL_VERTEX_SHADER, vertexSource);

					std::string pixelSource = parseShader("2D-ps.glsl");
					pixelID = compileShader(GL_FRAGMENT_SHADER, pixelSource);
				}
				break;

			case ShaderStyle::RT:
				{
					std::string vertexSource = parseShader("RT-vs.glsl");
					vertexID = compileShader(GL_VERTEX_SHADER, vertexSource);

					std::string pixelSource = parseShader("RT-ps.glsl");
					pixelID = compileShader(GL_FRAGMENT_SHADER, pixelSource);
				}
				break;

			default:
				CLV_ASSERT(false, "Unknown type! {0}", __func__);
				break;
		}

		CLV_ASSERT(vertexID != 0 && pixelID != 0, "Pixel or vertex shader not set!");

		glAttachShader(programID, vertexID);
		glLinkProgram(programID);
		glValidateProgram(programID);
		glDeleteShader(vertexID);

		glAttachShader(programID, pixelID);
		glLinkProgram(programID);
		glValidateProgram(programID);
		glDeleteShader(pixelID);
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

	uint32 GL4Shader::compileShader(uint32 type, const std::string& source){
		uint32 id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int32 result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if(result == GL_FALSE){
			int32 length;
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