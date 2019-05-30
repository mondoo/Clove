#include "clvpch.hpp"
#include "VertexBuffer.hpp"

namespace clv::gfx{
	VertexBuffer::VertexBuffer() = default;

	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept = default;

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept = default;

	VertexBuffer::~VertexBuffer() = default;
}