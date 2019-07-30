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

	//TODO: add an isBound function to the bindable class

	void GL4FrameBuffer::attachTexture(Texture& texture){
		glBindFramebuffer(GL_FRAMEBUFFER, renderID);
		//TODO
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	bool GL4FrameBuffer::isComplete() const{
		glBindFramebuffer(GL_FRAMEBUFFER, renderID);
		const bool result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return result;
	}
}