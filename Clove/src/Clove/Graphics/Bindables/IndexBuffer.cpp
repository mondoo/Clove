#include "clvpch.hpp"
#include "IndexBuffer.hpp"

namespace clv::gfx{
	IndexBuffer::IndexBuffer() = default;

	IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept = default;

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept = default;

	IndexBuffer::~IndexBuffer() = default;
}