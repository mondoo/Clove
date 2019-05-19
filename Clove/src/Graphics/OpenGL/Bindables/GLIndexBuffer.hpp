#pragma once

#include "Clove/Graphics/Bindables/IndexBuffer.hpp"

namespace clv::gfx{
	class Renderer;

	class GLIndexBuffer : public IndexBuffer{
		//VARIABLES
	private:
		unsigned int bufferID = 0;
		unsigned int count = 0;

		//FUNCTIONS
	public:
		GLIndexBuffer() = delete;
		GLIndexBuffer(const GLIndexBuffer& other) = delete;
		GLIndexBuffer(GLIndexBuffer&& other) noexcept;
		GLIndexBuffer& operator=(const GLIndexBuffer& other) = delete;
		GLIndexBuffer& operator=(GLIndexBuffer&& other) noexcept;
		~GLIndexBuffer();

		GLIndexBuffer(const std::vector<unsigned int>& indices);

		virtual void bind(Renderer& renderer) override;
		virtual void unbind() override;

		virtual unsigned int getIndexCount() const override;
	};
}
