#include "clvpch.hpp"
#include "GL4RenderTarget.hpp"

#include "Graphics/OpenGL-4/Bindables/GL4Texture.hpp"

#include <glad/glad.h>

namespace clv::gfx{
	GL4RenderTarget::GL4RenderTarget(GL4RenderTarget&& other) noexcept = default;

	GL4RenderTarget& GL4RenderTarget::operator=(GL4RenderTarget&& other) noexcept = default;

	GL4RenderTarget::~GL4RenderTarget(){
		glDeleteFramebuffers(1, &renderID);
		glDeleteRenderbuffers(1, &renderBufferID);
	}

	GL4RenderTarget::GL4RenderTarget(Texture& texture){
		glGenFramebuffers(1, &renderID);
		glBindFramebuffer(GL_FRAMEBUFFER, renderID);

		GL4Texture& glTexture = static_cast<GL4Texture&>(texture);
		const uint32 textureRenderID = glTexture.getRenderID();

		CLV_ASSERT(glTexture.getUsageType() == TextureUsage::RenderTarget, "Incorrect texture type");
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureRenderID, 0);

		//Setting up the depth buffer manually for GL render targets for now
		glGenRenderbuffers(1, &renderBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, glTexture.getWidth(), glTexture.getHeight());
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferID);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	const uint32 GL4RenderTarget::getRenderID() const{
		return renderID;
	}
}