#include "clvpch.hpp"
#include "GLVertexBuffer.hpp"

#include "Graphics/OpenGL/GLException.hpp"

#include <glad/glad.h>

namespace clv::gfx{
	GLVertexBuffer::GLVertexBuffer(GLVertexBuffer&& other) noexcept = default;

	GLVertexBuffer& GLVertexBuffer::operator=(GLVertexBuffer&& other) noexcept = default;

	GLVertexBuffer::~GLVertexBuffer(){
		GLCall(glDeleteBuffers(1, &bufferID));
	}

	GLVertexBuffer::GLVertexBuffer(const std::vector<float>& vertices){
		const unsigned int size = static_cast<unsigned int>(vertices.size() * sizeof(float));
		const void* data = vertices.data();

		GLCall(glGenBuffers(1, &bufferID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, bufferID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	void GLVertexBuffer::bind(Renderer& renderer){
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, bufferID));
	}

	void GLVertexBuffer::unbind(){
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}