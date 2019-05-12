#include "clvpch.hpp"
#include "GLIndexBuffer.hpp"

#include "GraphicsAPI/GL/GLException.hpp"

#include <glad/glad.h>

namespace clv{
	namespace gfx{
		GLIndexBuffer::GLIndexBuffer(GLIndexBuffer&& other) noexcept = default;

		GLIndexBuffer& GLIndexBuffer::operator=(GLIndexBuffer&& other) noexcept = default;
		
		GLIndexBuffer::~GLIndexBuffer(){
			GLCall(glDeleteBuffers(1, &rendererID));
		}

		GLIndexBuffer::GLIndexBuffer(const std::vector<unsigned int>& indices){
			count = indices.size();
			const auto dataSize = count * static_cast<unsigned int>(sizeof(unsigned int));

			GLCall(glGenBuffers(1, &rendererID));
			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
			GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, indices.data(), GL_STATIC_DRAW));
		}

		void GLIndexBuffer::bind(Renderer* renderer){
			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
		}

		void GLIndexBuffer::unbind(){
			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}
	}
}