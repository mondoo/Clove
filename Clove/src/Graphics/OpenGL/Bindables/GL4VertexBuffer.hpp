#pragma once

#include "Core/Graphics/Bindables/VertexBuffer.hpp"

namespace clv::gfx{
	class Shader;

	class GL4VertexBuffer : public VertexBuffer{
		//VARIABLES
	private:
		uint32 bufferID = 0;
		uint32 arrayID = 0;

		//FUNCTIONS
	public:
		GL4VertexBuffer() = delete;
		GL4VertexBuffer(const GL4VertexBuffer& other) = delete;
		GL4VertexBuffer(GL4VertexBuffer&& other) noexcept;
		GL4VertexBuffer& operator=(const GL4VertexBuffer& other) = delete;
		GL4VertexBuffer& operator=(GL4VertexBuffer&& other) noexcept;
		~GL4VertexBuffer();

		GL4VertexBuffer(const VertexBufferData& bufferData, Shader& shader);

		virtual void bind() override;
	};
}