#include "clvpch.hpp"
#include "GLVertexBuffer.hpp"

#include "Graphics/OpenGL/GLException.hpp"

#include <glad/glad.h>

namespace clv::gfx{
	GLVertexBuffer::GLVertexBuffer(GLVertexBuffer&& other) noexcept = default;

	GLVertexBuffer& GLVertexBuffer::operator=(GLVertexBuffer&& other) noexcept = default;

	GLVertexBuffer::~GLVertexBuffer(){
		glDeleteBuffers(1, &bufferID);
	}

	GLVertexBuffer::GLVertexBuffer(const std::vector<float>& vertices){
		const unsigned int size = static_cast<unsigned int>(vertices.size() * sizeof(float));
		const void* data = vertices.data();

		glGenBuffers(1, &bufferID);
		glBindBuffer(GL_ARRAY_BUFFER, bufferID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void GLVertexBuffer::bind(Renderer& renderer){
		glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	}

	void GLVertexBuffer::unbind(){
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}