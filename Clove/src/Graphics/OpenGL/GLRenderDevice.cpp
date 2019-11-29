#include "Clove/Graphics/OpenGL/GLRenderDevice.hpp"

#include "Clove/Graphics/OpenGL/GL.hpp"
#include "Clove/Graphics/OpenGL/Resources/GLBuffer.hpp"
#include "Clove/Graphics/OpenGL/Resources/GLTexture.hpp"
#include "Clove/Graphics/OpenGL/GLPipelineObject.hpp"
#include "Clove/Graphics/OpenGL/GLRenderTarget.hpp"
#include "Clove/Graphics/OpenGL/GLShader.hpp"
#include "Clove/Graphics/OpenGL/GLSurface.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	GLRenderDevice::GLRenderDevice() = default;

	GLRenderDevice::~GLRenderDevice() = default;

	void GLRenderDevice::bindIndexBuffer(const Buffer& buffer){
		const GLBuffer& glbuffer = static_cast<const GLBuffer&>(buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glbuffer.getBufferID());
	}

	void GLRenderDevice::bindVertexBuffer(const Buffer& buffer, const uint32 stride){
		const GLBuffer& glbuffer = static_cast<const GLBuffer&>(buffer);
		glBindVertexBuffer(0, glbuffer.getBufferID(), 0, stride);
	}

	void GLRenderDevice::bindShaderResourceBuffer(const Buffer& buffer, const ShaderType shaderType, const uint32 bindingPoint){
		const GLBuffer& glbuffer = static_cast<const GLBuffer&>(buffer);
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, glbuffer.getBufferID());
	}

	void GLRenderDevice::bindPipelineObject(const PipelineObject& pipelineObject){
		const GLPipelineObject& glPipelineObject = static_cast<const GLPipelineObject&>(pipelineObject);
		glBindVertexArray(glPipelineObject.getGLVertexArrayID());
		bindShader(*glPipelineObject.getShader());
	}

	void GLRenderDevice::bindTexture(const Texture* texture, const uint32 bindingPoint){
		const GLTexture* glTexture = static_cast<const GLTexture*>(texture);
		if(glTexture){
			glBindTextureUnit(bindingPoint, glTexture->getTextureID());
		} else{
			glBindTextureUnit(bindingPoint, 0);
		}
	}

	void GLRenderDevice::bindShader(const Shader& shader){
		const GLShader& glShader = static_cast<const GLShader&>(shader);
		glUseProgram(glShader.getProgramID());
	}

	void GLRenderDevice::updateBufferData(Buffer& buffer, const void* data){
		const GLBuffer& glbuffer = static_cast<const GLBuffer&>(buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, glbuffer.getBufferID());
		glBufferSubData(GL_UNIFORM_BUFFER, 0, glbuffer.getDescriptor().bufferSize, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void GLRenderDevice::makeSurfaceCurrent(const std::shared_ptr<Surface>& surface){
		auto glSurface = std::static_pointer_cast<GLSurface>(surface);
		glSurface->makeCurrent();
		//Not the ideal place but it has to happen after the surface(context) is created and made current
		postContextCreatedInitialise();
	}
	
	void GLRenderDevice::setRenderTarget(const RenderTarget* renderTarget){
		if(renderTarget){
			const GLRenderTarget* glRenderTarget = static_cast<const GLRenderTarget*>(renderTarget);
			glBindFramebuffer(GL_FRAMEBUFFER, glRenderTarget->getGLFrameBufferID());
		} else{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void GLRenderDevice::resetRenderTargetToDefault(){
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLRenderDevice::setViewport(const Viewport& viewport){
		glViewport(static_cast<GLint>(viewport.x), static_cast<GLint>(viewport.y), static_cast<GLsizei>(viewport.width), static_cast<GLsizei>(viewport.height));
	}

	void GLRenderDevice::clear(){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLRenderDevice::drawIndexed(const uint32 count){
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, nullptr);
	}

	void GLRenderDevice::setClearColour(const mth::vec4f& colour){
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
}