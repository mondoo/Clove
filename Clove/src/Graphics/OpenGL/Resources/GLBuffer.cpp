#include "Clove/Graphics/OpenGL/Resources/GLBuffer.hpp"

namespace clv::gfx::ogl{
	static GLenum getGLBufferType(BufferType cloveType){
		switch (cloveType){
		case BufferType::IndexBuffer:
			return GL_ELEMENT_ARRAY_BUFFER;
		case BufferType::VertexBuffer:
			return GL_ARRAY_BUFFER;
		case BufferType::ShaderResourceBuffer:
			return GL_UNIFORM_BUFFER;
		default:
			CLV_ASSERT(false, "Unkown buffer type in {0}", CLV_FUNCTION_NAME);
			return 0;
		}
	}

	static GLenum getGLBufferUsage(BufferUsage cloveUsage){
		switch (cloveUsage){
		case BufferUsage::Default:
			return GL_STATIC_DRAW;
		case BufferUsage::Dynamic:
			return GL_DYNAMIC_DRAW;
		default:
			CLV_ASSERT(false, "Unkown buffer usage in {0}", CLV_FUNCTION_NAME);
			return 0;
		}
	}

	GLBuffer::GLBuffer(const BufferDescriptor& descriptor, const void* data)
		: descriptor(descriptor){
		glGenBuffers(1, &bufferID);

		const GLenum bufferType = getGLBufferType(descriptor.bufferType);
		const GLenum bufferUsage = getGLBufferUsage(descriptor.bufferUsage);

		glBindBuffer(bufferType, bufferID);
		glBufferData(bufferType, descriptor.bufferSize, data, bufferUsage);
		glBindBuffer(bufferType, 0);
	}

	GLBuffer::GLBuffer(GLBuffer&& other) noexcept = default;

	GLBuffer& GLBuffer::operator=(GLBuffer&& other) noexcept = default;

	GLBuffer::~GLBuffer(){
		glDeleteBuffers(1, &bufferID);
	}

	const BufferDescriptor& GLBuffer::getDescriptor() const{
		return descriptor;
	}

	void GLBuffer::updateData(const void* data){
		glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, descriptor.bufferSize, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	GLuint GLBuffer::getBufferID() const{
		return bufferID;
	}
}