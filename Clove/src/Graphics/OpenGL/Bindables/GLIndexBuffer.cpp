#include "clvpch.hpp"
#include "GLIndexBuffer.hpp"

#include "Graphics/OpenGL/GLException.hpp"

#include <glad/glad.h>

namespace clv::gfx{
	GLIndexBuffer::GLIndexBuffer(GLIndexBuffer&& other) noexcept = default;

	GLIndexBuffer& GLIndexBuffer::operator=(GLIndexBuffer&& other) noexcept = default;

	GLIndexBuffer::~GLIndexBuffer(){
		GLCall(glDeleteBuffers(1, &bufferID));
	}

	GLIndexBuffer::GLIndexBuffer(const std::vector<unsigned int>& indices){
		count = indices.size();
		const auto dataSize = count * static_cast<GLsizeiptr>(sizeof(unsigned int));

		GLCall(glGenBuffers(1, &bufferID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, indices.data(), GL_STATIC_DRAW));
	}

	void GLIndexBuffer::bind(Renderer& renderer){
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID));
	}

	void GLIndexBuffer::unbind(){
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	unsigned int GLIndexBuffer::getIndexCount() const{
		return count;
	}
}