#include "clvpch.hpp"
#include "FrameBuffer.hpp"

namespace clv::gfx{
	FrameBuffer::FrameBuffer() = default;

	FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept = default;

	FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept = default;

	FrameBuffer::~FrameBuffer() = default;
}