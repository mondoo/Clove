#include "GLBuffer.hpp"

namespace clv::gfx::ogl{
	GLBuffer::GLBuffer(const BufferDescriptor& descriptor, void* data)
		: descriptor(descriptor){
		glGenBuffers(1, &bufferID);

		const GLenum bufferType = getGLBufferType(descriptor.bufferType);
		const GLenum bufferUsage = getGLBufferUsage(descriptor.bufferUsage);

		glBindBuffer(bufferType, bufferID);
		glBufferData(bufferType, descriptor.bufferSize, data, bufferUsage);
		glBindBuffer(bufferType, 0);
	}

	GLBuffer::GLBuffer(GLBuffer&& other) = default;

	GLBuffer& GLBuffer::operator=(GLBuffer&& other) = default;

	GLBuffer::~GLBuffer(){
		glDeleteBuffers(1, &bufferID);
	}

	GLuint GLBuffer::getBufferID() const{
		return bufferID;
	}

	const BufferDescriptor& GLBuffer::getDescriptor() const{
		return descriptor;
	}

	GLenum GLBuffer::getGLBufferType(BufferType cloveType){
		switch(cloveType){
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

	GLenum GLBuffer::getGLBufferUsage(BufferUsage cloveUsage){
		switch(cloveUsage){
			case BufferUsage::Default:
				return GL_STATIC_DRAW;
			case BufferUsage::Dynamic:
				return GL_DYNAMIC_DRAW;
			default:
				CLV_ASSERT(false, "Unkown buffer usage in {0}", CLV_FUNCTION_NAME);
				return 0;
		}
	}
}