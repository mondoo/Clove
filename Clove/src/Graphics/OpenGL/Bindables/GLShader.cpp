#include "clvpch.hpp"
#include "GLShader.hpp"

#include "Graphics/OpenGL/GLException.hpp"
#include "Clove/Graphics/Bindables/Texture.hpp"

#include <glad/glad.h>
#include <fstream>
#include <sstream>

namespace clv::gfx{
	GLShader::GLShader()
		: programID(glCreateProgram())
		, modelUniform("model")
		, normalMatrixUniform("normalMatrix")
		, diffuseSlotUniform("material.diffuse", TBP_Diffuse) //This is tell opengl which slot this texture is in
		, specularSlotUniform("material.specular", TBP_Specular)
		, matShininess("material.shininess", 32.0f){
	}

	GLShader::~GLShader(){
		GLCall(glDeleteProgram(programID));
	}

	void GLShader::bind(Renderer& renderer){
		GLCall(glUseProgram(programID));

		modelUniform.bind(programID);
		normalMatrixUniform.bind(programID);
		diffuseSlotUniform.bind(programID);
		specularSlotUniform.bind(programID);
		matShininess.bind(programID);
	}

	void GLShader::unbind(){
		GLCall(glUseProgram(0));
	}

	void GLShader::attachShader(ShaderTypes type){
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
		modelUniform.update(model);
		normalMatrixUniform.update(math::transpose(math::inverse(model)));
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
}