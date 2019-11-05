#pragma once

#include "Core/Graphics/Resources/Buffer.hpp"

//#include "Core/Graphics/VertexLayout.hpp"

namespace clv::gfx{
	class VertexBuffer : public Buffer{
		//VARIABLES
	protected:
		//VertexBufferData bufferData; //TODO: Chuck back in?

		//FUNCTIONS
	public:
		VertexBuffer()/* = delete*/ = default;
		VertexBuffer(const VertexBuffer& other) = delete;
		VertexBuffer(VertexBuffer&& other) noexcept = default;
		VertexBuffer& operator=(const VertexBuffer& other) = delete;
		VertexBuffer& operator=(VertexBuffer&& other) noexcept = default;
		virtual ~VertexBuffer() = default;

		//VertexBuffer(const VertexBufferData& bufferData);
	};
}

