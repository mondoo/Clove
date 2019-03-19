#include "clvpch.h"
#include "RenderLayer.h"

//using glad for now
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>
#include <sstream>

#define GLCall(x)	GLClearError();\
					x;\
					CLV_CORE_ASSERT(GLLogCall(), "{0} {1} {2}", #x, __FILE__, __LINE__)

static void GLClearError(){
	while(glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(){
	while(GLenum error = glGetError()){
		CLV_CORE_ERROR("OpenGL Error! ({0})", error);
		return false;
	}
	return true;
}

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
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE){
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = new char [length];
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		CLV_CORE_ERROR("Failed to compile {0} shader! {1}", type == GL_VERTEX_SHADER ? "vertex" : "fragment", message);
		GLCall(glDeleteShader(id));
		delete [] message;
	}

	return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader){
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	//should also be technically detaching the shader (deleting the source code)
	//although not doing that means it is easier debug

	return program;
}

namespace clv{
	void RenderLayer::onAttach(){
		CLV_CORE_TRACE("GL version: {0}", glGetString(GL_VERSION));

		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));

		//Vertex Buffer
		GLCall(glGenBuffers(1, &bufferID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, bufferID)); //this selects the buffer we want to draw (think of these as layers in photoshop)
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW)); //This can happen any time after the buffer is bound (gives the bound buffer the data)
	
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
		//0 is the vertex buffer we bind to the array, if we made a second we'd but 1 here to bind that other buffer

		//Index Buffer
		GLCall(glGenBuffers(1, &ibo));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW));

		//Shaders
		ShaderProgramSource source = parseShader("F:/Clove/Clove/res/Shaders/Basic.shader"); //temp: relative path didn't work
		//CLV_CORE_TRACE("\nVERTEX: \n{0}", source.vertexSource);
		//CLV_CORE_TRACE("\nFRAGMENT: \n{0}", source.fragmentSource);

		shader = createShader(source.vertexSource, source.fragmentSource);
		GLCall(glUseProgram(shader)); //Tell open gl to use this shader (for drawing or unforming)

		GLCall(location = glGetUniformLocation(shader, "u_Color"));
		if(location == -1){
			CLV_CORE_WARN("Could not get uniform or uniform is not used");
		}
		/*GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));*/ //uniforms can send data per update (compared to vertex attributes which are defined when creating the vertex)
	
		GLCall(glBindVertexArray(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		GLCall(glUseProgram(0));
	}

	void RenderLayer::onDetach(){
		GLCall(glDeleteProgram(shader));
	}

	void RenderLayer::onUpdate(){
		//Gl is initialised in window

		//glClearColor(1, 0, 1, 1);
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		if(r > 1.0f || r < 0.0f){
			increment = -increment;
		}
		r += increment;

		GLCall(glUseProgram(shader));
		GLCall(glUniform4f(location, r, 1.0f, 1.0f, 1.0f));
		
		/*GLCall(glBindBuffer(GL_ARRAY_BUFFER, bufferID));
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));*/
		
		//When using a vertex array we no longer need to do the above block of code because we bound that data (earlier up) to the vertex array
		GLCall(glBindVertexArray(vao));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

		GLCall(glDrawElements(GL_TRIANGLES, sizeof(indicies) / sizeof(unsigned int), GL_UNSIGNED_INT, nullptr)); //nullptr because we bound our indicies to a buffer

		//swap buffers happen in window
	}
}