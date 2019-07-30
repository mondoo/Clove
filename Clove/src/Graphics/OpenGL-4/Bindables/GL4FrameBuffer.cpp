#include "clvpch.hpp"
#include "GL4FrameBuffer.hpp"

namespace clv::gfx{
	GL4FrameBuffer::GL4FrameBuffer() = default;

	GL4FrameBuffer::GL4FrameBuffer(GL4FrameBuffer&& other) noexcept = default;

	GL4FrameBuffer& GL4FrameBuffer::operator=(GL4FrameBuffer&& other) noexcept = default;

	GL4FrameBuffer::~GL4FrameBuffer() = default;

	void GL4FrameBuffer::bind(){
		//TODO
	}
}