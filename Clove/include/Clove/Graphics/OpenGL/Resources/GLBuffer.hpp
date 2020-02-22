#pragma once

#include "Clove/Graphics/Core/Resources/Buffer.hpp"

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
		GLBuffer(GLBuffer&& other) noexcept;

		GLBuffer& operator=(const GLBuffer& other) noexcept = delete;
		GLBuffer& operator=(GLBuffer&& other) noexcept;

		virtual ~GLBuffer();

		virtual const BufferDescriptor& getDescriptor() const override;

		virtual void updateData(const void* data) override;

		GLuint getBufferID() const;
	};
}