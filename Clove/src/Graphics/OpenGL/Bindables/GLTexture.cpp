#include "clvpch.hpp"
#include "GLTexture.hpp"

#include "Graphics/OpenGL/GLException.hpp"

#include <glad/glad.h>
#include <stb_image.h>

namespace clv::gfx{
	GLTexture::GLTexture(GLTexture&& other) noexcept = default;

	GLTexture& GLTexture::operator=(GLTexture&& other) noexcept = default;

	GLTexture::~GLTexture(){
		GLCall(glDeleteTextures(1, &rendererID));
	}

	GLTexture::GLTexture(const std::string& filePath, unsigned int bindingPoint)
		: filePath(filePath)
		, bindingPoint(bindingPoint){
		stbi_set_flip_vertically_on_load(1); //Opengl expects our texture to start on the bottom left
		unsigned char* localBuffer = stbi_load(filePath.c_str(), &width, &height, &BPP, 4); //4 = RGBA

		GLCall(glGenTextures(1, &rendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, rendererID));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		if(localBuffer){
			stbi_image_free(localBuffer);
		}
	}

	void GLTexture::bind(Renderer& renderer){
		GLCall(glActiveTexture(GL_TEXTURE0 + bindingPoint));
		GLCall(glBindTexture(GL_TEXTURE_2D, rendererID));
	}

	void GLTexture::unbind(){
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	int GLTexture::getWidth() const{
		return width;
	}

	int GLTexture::getHeight() const{
		return height;
	}
}