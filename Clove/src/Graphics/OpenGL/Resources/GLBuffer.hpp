#pragma once

#include "Core/Graphics/Resources/Buffer.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	class GLBuffer : public Buffer{
		//VARIABLES
	private:
		BufferDescriptor descriptor;

		GLuint bufferID = 0;

		//FUNCTIONS
	public:
		GLBuffer() = delete;
		GLBuffer(const BufferDescriptor& descriptor, const void* data);
		GLBuffer(const GLBuffer& other) = delete;
		GLBuffer(GLBuffer&& other);
		GLBuffer& operator=(const GLBuffer& other) = delete;
		GLBuffer& operator=(GLBuffer&& other);
		virtual ~GLBuffer();

		GLuint getBufferID() const;

		virtual const BufferDescriptor& getDescriptor() const override;

	private:
		GLenum getGLBufferType(BufferType cloveType);
		GLenum getGLBufferUsage(BufferUsage cloveUsage);
	};
}