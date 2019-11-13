#include "GLRenderDevice.hpp"

#include "Graphics/OpenGL/GL.hpp"
#include "Graphics/OpenGL/Resources/GLBuffer.hpp"
#include "Graphics/OpenGL/Resources/GLTexture.hpp"
#include "Graphics/OpenGL/GLPipelineObject.hpp"
#include "Graphics/OpenGL/GLRenderTarget.hpp"
#include "Graphics/OpenGL/GLShader.hpp"
#include "Graphics/OpenGL/GLSurface.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	GLRenderDevice::GLRenderDevice() = default;

	GLRenderDevice::~GLRenderDevice() = default;

	void GLRenderDevice::bindIndexBuffer(const Buffer& buffer){
		const GLBuffer& glbuffer = static_cast<const GLBuffer&>(buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glbuffer.getBufferID());
	}

	void GLRenderDevice::bindVertexBuffer(const Buffer& buffer){
		const GLBuffer& glbuffer = static_cast<const GLBuffer&>(buffer);
		glBindVertexBuffer(0, glbuffer.getBufferID(), 0, buffer.getDescriptor().elementSize);
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

	void GLRenderDevice::bindTexture(const Texture& texture, const uint32 bindingPoint){
		const GLTexture& glTexture = static_cast<const GLTexture&>(texture);
		glBindTextureUnit(bindingPoint, glTexture.getTextureID());
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

	void GLRenderDevice::makeSurfaceCurrent(Surface& surface){
		GLSurface& glSurface = static_cast<GLSurface&>(surface);
		glSurface.makeCurrent();
		//Not the ideal place but it has to happen after the surface(context) is created and made current
		postContextCreatedInitialise();
	}
	
	void GLRenderDevice::setRenderTarget(RenderTarget& renderTarget){
		GLRenderTarget& glRenderTarget = static_cast<GLRenderTarget&>(renderTarget);
		glBindFramebuffer(GL_FRAMEBUFFER, glRenderTarget.getGLFrameBufferID());
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