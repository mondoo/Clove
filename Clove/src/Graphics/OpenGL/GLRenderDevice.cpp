#include "GLRenderDevice.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	GLRenderDevice::GLRenderDevice() = default;

	GLRenderDevice::~GLRenderDevice() = default;

	void GLRenderDevice::bindIndexBuffer(IndexBuffer& buffer){
		//TODO:
	}

	void GLRenderDevice::bindVertexBuffer(VertexBuffer& buffer){
		//TODO:
	}

	void GLRenderDevice::bindShaderResource(ShaderResource& resource){
		//TODO:
	}

	void GLRenderDevice::bindTexture(Texture& texture){
		//TODO:
	}

	void GLRenderDevice::bindShader(Shader& shader){
		//TODO:
	}

	void GLRenderDevice::makeSurfaceCurrent(const Surface& surface){
		//TODO:
	}

	void GLRenderDevice::setRenderTarget(RenderTarget& renderTarget){
		GL4RenderTarget& glRenderTarget = static_cast<GL4RenderTarget&>(renderTarget);
		glBindFramebuffer(GL_FRAMEBUFFER, glRenderTarget.getRenderID());
	}

	void GLRenderDevice::resetRenderTargetToDefault(){
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLRenderDevice::setViewport(const Viewport& viewport){
		glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	}

	void GLRenderDevice::clear(){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLRenderDevice::drawIndexed(const uint32 count){
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, nullptr);
	}

	void GLRenderDevice::setClearColour(const math::Vector4f& colour){
		glClearColor(colour.r, colour.g, colour.b, colour.a);
	}

	void GLRenderDevice::setDepthBuffer(bool enabled){
		if(enabled){
			glEnable(GL_DEPTH_TEST);
		} else{
			glDisable(GL_DEPTH_TEST);
		}
	}

	void GLRenderDevice::setBlendState(bool enabled){
		if(enabled){
			glEnable(GL_BLEND);
		} else{
			glDisable(GL_BLEND);
		}
	}

	void GLRenderDevice::removeCurrentGeometryShader(){
		glUseProgram(0);
	}

	void GLRenderDevice::removeTextureAtSlot(uint32 slot){
		glBindTextureUnit(slot, 0);
	}
}