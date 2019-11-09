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

	GL4RenderTarget::GL4RenderTarget(Texture* colourTexture, Texture* depthStencilTexture){
		glGenFramebuffers(1, &renderID);
		glBindFramebuffer(GL_FRAMEBUFFER, renderID);

		CLV_ASSERT(colourTexture != nullptr || depthStencilTexture != nullptr, "{0}: Render target needs at least one valid texture", CLV_FUNCTION_NAME);

		//Colour buffer
		if(colourTexture){
			CLV_ASSERT(colourTexture->getUsageType() == TextureUsage::RenderTarget_Colour, "Incorrect texture type for colour texture");
			
			GLTexture* glColourTexture = static_cast<GLTexture*>(colourTexture);
			const uint32 textureRenderID = glColourTexture->getTextureID();
			
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureRenderID, 0);
		} else{
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}

		//Depth / Stencil buffer
		if(depthStencilTexture){
			//NOTE: Asserting here but this will need to change when adding stencil buffers
			CLV_ASSERT(depthStencilTexture->getUsageType() == TextureUsage::RenderTarget_Depth, "Incorrect texture type for depth stencil texture");

			GLTexture* gldepthStencilTexture = static_cast<GLTexture*>(depthStencilTexture);
			const uint32 textureRenderID = gldepthStencilTexture->getTextureID();

			if(depthStencilTexture->getTextureStyle() == TextureStyle::Default){
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureRenderID, 0);
			} else if(depthStencilTexture->getTextureStyle() == TextureStyle::Cubemap){
				glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureRenderID, 0);
			}
		} else{
			glGenRenderbuffers(1, &renderBufferID);
			glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, colourTexture->getWidth(), colourTexture->getHeight());
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferID);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	const uint32 GL4RenderTarget::getRenderID() const{
		return renderID;
	}
}