#include "clvpch.hpp"
#include "IndexBuffer.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Rendering/API/GLHelpers.hpp"

#include <glad/glad.h>

namespace clv{
	IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept{
		rendererID = other.rendererID;
		count = other.count;

		other.rendererID = 0;
		other.count = 0;
	}

	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int inCount)
		: count(inCount){
		GLCall(glGenBuffers(1, &rendererID));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}

	IndexBuffer::~IndexBuffer(){
		GLCall(glDeleteBuffers(1, &rendererID));
	}

	void IndexBuffer::bind() const{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
	}

	void IndexBuffer::unbind() const{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept{
		rendererID = other.rendererID;
		count = other.count;

		other.rendererID = 0;
		other.count = 0;

		return *this;
	}
}