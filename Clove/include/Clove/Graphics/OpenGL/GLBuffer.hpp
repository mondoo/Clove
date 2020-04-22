#pragma once

#include "Clove/Graphics/Buffer.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	class GLBuffer : public Buffer{
		//VARIABLES
	private:
		std::shared_ptr<GraphicsFactory> factory;

		BufferDescriptor descriptor;

		GLuint bufferID = 0;

		//FUNCTIONS
	public:
		GLBuffer() = delete;
		GLBuffer(std::shared_ptr<GraphicsFactory> factory, const BufferDescriptor& descriptor, const void* data);

		GLBuffer(const GLBuffer& other) = delete;
		GLBuffer(GLBuffer&& other) noexcept;

		GLBuffer& operator=(const GLBuffer& other) noexcept = delete;
		GLBuffer& operator=(GLBuffer&& other) noexcept;

		~GLBuffer();

		const std::shared_ptr<GraphicsFactory>& getFactory() const override;

		const BufferDescriptor& getDescriptor() const override;

		void updateData(const void* data) override;

		GLuint getBufferID() const;
	};
}