#pragma once

#include "Core/Graphics/Bindables/IndexBuffer.hpp"

namespace clv::gfx{
	class Renderer;

	class GL4IndexBuffer : public IndexBuffer{
		//VARIABLES
	private:
		uint32 bufferID = 0;
		uint32 count = 0;

		//FUNCTIONS
	public:
		GL4IndexBuffer() = delete;
		GL4IndexBuffer(const GL4IndexBuffer& other) = delete;
		GL4IndexBuffer(GL4IndexBuffer&& other) noexcept;
		GL4IndexBuffer& operator=(const GL4IndexBuffer& other) = delete;
		GL4IndexBuffer& operator=(GL4IndexBuffer&& other) noexcept;
		~GL4IndexBuffer();

		GL4IndexBuffer(const std::vector<uint32>& indices);

		virtual void bind() override;

		virtual uint32 getIndexCount() const override;
		uint32 getBufferID() const;
	};
}
