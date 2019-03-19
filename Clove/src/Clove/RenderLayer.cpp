#include "clvpch.h"
#include "RenderLayer.h"

//using glad for now
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource{
	std::string vertexSource;
	std::string fragmentSource;
};

static ShaderProgramSource parseShader(const std::string& filepath){
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

static unsigned int compileShader(unsigned int type, const std::string& source){
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE){
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char [length];
		glGetShaderInfoLog(id, length, &length, message);
		CLV_CORE_ERROR("Failed to compile {0} shader! {1}", type == GL_VERTEX_SHADER ? "vertex" : "fragment", message);
		glDeleteShader(id);
		delete [] message;
	}

	return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader){
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	//should also be technically detaching the shader (deleting the source code)
	//although not doing that means it is easier debug

	return program;
}

namespace clv{
	void RenderLayer::onAttach(){
		CLV_CORE_TRACE("GL version: {0}", glGetString(GL_VERSION));

		//Vertex Buffer
		unsigned int bufferID = 0;
		glGenBuffers(1, &bufferID);
		glBindBuffer(GL_ARRAY_BUFFER, bufferID); //this selects the buffer we want to draw (think of these as layers in photoshop)
		glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW); //This can happen any time after the buffer is bound (gives the bound buffer the data)
	
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

		//Index Buffer
		unsigned int ibo = 0;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

		//Shaders
		ShaderProgramSource source = parseShader("F:/Clove/Clove/res/Shaders/Basic.shader"); //temp: relative path didn't work
		CLV_CORE_TRACE("\nVERTEX: \n{0}", source.vertexSource);
		CLV_CORE_TRACE("\nFRAGMENT: \n{0}", source.fragmentSource);

		shader = createShader(source.vertexSource, source.fragmentSource);
		glUseProgram(shader);
	}

	void RenderLayer::onDetach(){
		glDeleteProgram(shader);
	}

	void RenderLayer::onUpdate(){
		//Gl is initialised in window

		//glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, sizeof(indicies) / sizeof(unsigned int), GL_UNSIGNED_INT, nullptr); //nullptr because we bound our indicies to a buffer
		

		//swap buffers happen in window
	}
}