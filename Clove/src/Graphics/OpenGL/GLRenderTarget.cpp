#include "GLRenderTarget.hpp"

#include "Graphics/OpenGL/Resources/GLTexture.hpp"

namespace clv::gfx::ogl{
	GLRenderTarget::GLRenderTarget(Texture* colourTexture, Texture* depthStencilTexture){
		glGenFramebuffers(1, &frameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

		CLV_ASSERT(colourTexture != nullptr || depthStencilTexture != nullptr, "{0}: Render target needs at least one valid texture", CLV_FUNCTION_NAME);

		//Colour buffer
		if(colourTexture){
			GLTexture* glColourTexture = static_cast<GLTexture*>(colourTexture);
			const TextureUsage usage = glColourTexture->getDescriptor().usage;
			const uint32 textureRenderID = glColourTexture->getTextureID();

			CLV_ASSERT(usage == TextureUsage::RenderTarget_Colour, "Incorrect texture type for colour texture");

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureRenderID, 0);
		} else{
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}

		//Depth / Stencil buffer
		if(depthStencilTexture){
			GLTexture* gldepthStencilTexture = static_cast<GLTexture*>(depthStencilTexture);
			const TextureUsage usage = gldepthStencilTexture->getDescriptor().usage;
			const TextureStyle style = gldepthStencilTexture->getDescriptor().style;
			const uint32 textureRenderID = gldepthStencilTexture->getTextureID();

			//NOTE: Asserting here but this will need to change when adding stencil buffers
			CLV_ASSERT(usage == TextureUsage::RenderTarget_Depth, "Incorrect texture type for depth stencil texture");

			if(style == TextureStyle::Default){
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureRenderID, 0);
			} else if(style == TextureStyle::Cubemap){
				glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureRenderID, 0);
			}
		} else{
			const math::Vector2f dimensions = colourTexture->getDescriptor().dimensions;
			glGenRenderbuffers(1, &renderBufferID);
			glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, dimensions.x, dimensions.y);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferID);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GLRenderTarget::GLRenderTarget(GLRenderTarget&& other) noexcept = default;

	GLRenderTarget& GLRenderTarget::operator=(GLRenderTarget&& other) noexcept = default;

	GLRenderTarget::~GLRenderTarget(){
		glDeleteFramebuffers(1, &frameBufferID);
		glDeleteRenderbuffers(1, &renderBufferID);
	}

	const uint32 GLRenderTarget::getGLFrameBufferID() const{
		return frameBufferID;
	}
}