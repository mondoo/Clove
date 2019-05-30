#include "clvpch.hpp"
#include "GLIndexBuffer.hpp"

#include "Graphics/OpenGL/GLException.hpp"

#include <glad/glad.h>

namespace clv::gfx{
	GLIndexBuffer::GLIndexBuffer(GLIndexBuffer&& other) noexcept = default;

	GLIndexBuffer& GLIndexBuffer::operator=(GLIndexBuffer&& other) noexcept = default;

	GLIndexBuffer::~GLIndexBuffer(){
		glDeleteBuffers(1, &bufferID);
	}

	GLIndexBuffer::GLIndexBuffer(const std::vector<unsigned int>& indices){
		count = indices.size();
		const auto dataSize = count * static_cast<GLsizeiptr>(sizeof(unsigned int));

		glGenBuffers(1, &bufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, indices.data(), GL_STATIC_DRAW);
	}

	void GLIndexBuffer::bind(Renderer& renderer){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
	}

	void GLIndexBuffer::unbind(){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	unsigned int GLIndexBuffer::getIndexCount() const{
		return count;
	}
}