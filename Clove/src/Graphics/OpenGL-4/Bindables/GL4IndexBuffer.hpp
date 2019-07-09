#pragma once

#include "Clove/Graphics/Bindables/IndexBuffer.hpp"

namespace clv::gfx{
	class Renderer;

	class GL4IndexBuffer : public IndexBuffer{
		//VARIABLES
	private:
		unsigned int bufferID = 0;
		unsigned int count = 0;

		//FUNCTIONS
	public:
		GL4IndexBuffer() = delete;
		GL4IndexBuffer(const GL4IndexBuffer& other) = delete;
		GL4IndexBuffer(GL4IndexBuffer&& other) noexcept;
		GL4IndexBuffer& operator=(const GL4IndexBuffer& other) = delete;
		GL4IndexBuffer& operator=(GL4IndexBuffer&& other) noexcept;
		~GL4IndexBuffer();

		GL4IndexBuffer(const std::vector<unsigned int>& indices);

		virtual void bind() override;
		virtual void unbind() override;

		virtual unsigned int getIndexCount() const override;
		unsigned int getBufferID() const;
	};
}
