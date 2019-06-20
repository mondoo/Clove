#include "clvpch.hpp"
#include "GL4VertexBuffer.hpp"

#include <glad/glad.h>

namespace clv::gfx{
	GL4VertexBuffer::GL4VertexBuffer(GL4VertexBuffer&& other) noexcept = default;

	GL4VertexBuffer& GL4VertexBuffer::operator=(GL4VertexBuffer&& other) noexcept = default;

	GL4VertexBuffer::~GL4VertexBuffer(){
		glDeleteBuffers(1, &bufferID);
	}

	//GL4VertexBuffer::GL4VertexBuffer(const std::vector<Vertex>& vertices){
	//	const unsigned int size = static_cast<unsigned int>(sizeof(vertices) * sizeof(Vertex));
	//	const void* data = vertices.data();

	//	glGenBuffers(1, &bufferID);
	//	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	//	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	//}

	void GL4VertexBuffer::bind(Renderer& renderer){
		glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	}

	void GL4VertexBuffer::unbind(){
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}