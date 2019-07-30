#include "clvpch.hpp"
#include "GL4FrameBuffer.hpp"

#include <glad/glad.h>

namespace clv::gfx{
	GL4FrameBuffer::GL4FrameBuffer(){
		glGenFramebuffers(1, &renderID);
	}

	GL4FrameBuffer::GL4FrameBuffer(GL4FrameBuffer&& other) noexcept = default;

	GL4FrameBuffer& GL4FrameBuffer::operator=(GL4FrameBuffer&& other) noexcept = default;

	GL4FrameBuffer::~GL4FrameBuffer(){
		glDeleteFramebuffers(1, &renderID);
	}

	void GL4FrameBuffer::bind(){
		glBindFramebuffer(GL_FRAMEBUFFER, renderID);
	}
}