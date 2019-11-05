#include "VertexBuffer.hpp"

namespace clv::gfx{
	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept = default;

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept = default;

	VertexBuffer::~VertexBuffer() = default;
	
	VertexBuffer::VertexBuffer(const VertexBufferData& bufferData)
		: bufferData(bufferData){
	}
}