#include "Clove/Graphics/OpenGL/GLCommandBuffer.hpp"

#include "Clove/Graphics/OpenGL/Resources/GLBuffer.hpp"
#include "Clove/Graphics/OpenGL/Resources/GLTexture.hpp"
#include "Clove/Graphics/OpenGL/GLPipelineObject.hpp"
#include "Clove/Graphics/OpenGL/GLRenderTarget.hpp"
#include "Clove/Graphics/OpenGL/GLShader.hpp"
#include "Clove/Graphics/OpenGL/GLSurface.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	GLCommandBuffer::GLCommandBuffer(const std::shared_ptr<RenderTarget>& renderTarget){
		glRenderTarget = std::static_pointer_cast<GLRenderTarget>(renderTarget);
	}

	GLCommandBuffer::GLCommandBuffer(const std::shared_ptr<Surface>& surface){
		glRenderTarget = std::static_pointer_cast<GLRenderTarget>(surface->getRenderTarget());
	}

	GLCommandBuffer::GLCommandBuffer(GLCommandBuffer&& other) noexcept = default;

	GLCommandBuffer& GLCommandBuffer::operator=(GLCommandBuffer&& other) noexcept = default;

	GLCommandBuffer::~GLCommandBuffer() = default;

	void GLCommandBuffer::beginEncoding(){
		const auto beginCommand = [glRenderTarget = glRenderTarget.get()](){
			glBindFramebuffer(GL_FRAMEBUFFER, glRenderTarget->getGLFrameBufferID());
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		};

		commands.push_back(beginCommand);
	}

	void GLCommandBuffer::bindIndexBuffer(const Buffer& buffer){
		const auto bindIBCommand = [&buffer](){
			const GLBuffer& glbuffer = static_cast<const GLBuffer&>(buffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glbuffer.getBufferID());
		};

		commands.push_back(bindIBCommand);
	}

	void GLCommandBuffer::bindVertexBuffer(const Buffer& buffer, const uint32 stride){
		const auto bindVBCommand = [&buffer, stride](){
			const GLBuffer& glbuffer = static_cast<const GLBuffer&>(buffer);
			glBindVertexBuffer(0, glbuffer.getBufferID(), 0, stride);
		};

		commands.push_back(bindVBCommand);
	}

	void GLCommandBuffer::bindShaderResourceBuffer(const Buffer& buffer, const ShaderType shaderType, const uint32 bindingPoint){
		const auto bindSRBCommand = [&buffer, shaderType, bindingPoint](){
			const GLBuffer& glbuffer = static_cast<const GLBuffer&>(buffer);
			glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, glbuffer.getBufferID());
		};

		commands.push_back(bindSRBCommand);
	}

	void GLCommandBuffer::bindPipelineObject(const PipelineObject& pipelineObject){
		const auto bindPOCommand = [&pipelineObject](){
			const GLPipelineObject& glPipelineObject = static_cast<const GLPipelineObject&>(pipelineObject);
			glBindVertexArray(glPipelineObject.getGLVertexArrayID());

			const auto glShader = std::static_pointer_cast<GLShader>(glPipelineObject.getShader());
			glUseProgram(glShader->getProgramID());

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			if(glPipelineObject.isBlendEnabled()){
				glEnable(GL_BLEND);
			} else{
				glDisable(GL_BLEND);
			}
		};

		commands.push_back(bindPOCommand);
	}

	void GLCommandBuffer::bindTexture(const Texture* texture, const uint32 bindingPoint){
		const auto bindTextureCommand = [texture, bindingPoint](){
			if(const GLTexture* glTexture = static_cast<const GLTexture*>(texture)){
				glBindTextureUnit(bindingPoint, glTexture->getTextureID());
			} else{
				glBindTextureUnit(bindingPoint, 0);
			}
		};

		commands.push_back(bindTextureCommand);
	}

	void GLCommandBuffer::setViewport(const Viewport& viewport){
		const auto setVPCommand = [&viewport](){
			glViewport(static_cast<GLint>(viewport.x), static_cast<GLint>(viewport.y), static_cast<GLsizei>(viewport.width), static_cast<GLsizei>(viewport.height));
		};

		commands.push_back(setVPCommand);
	}

	void GLCommandBuffer::setDepthEnabled(bool enabled){
		const auto setDECommand = [enabled](){
			glDepthFunc(GL_LESS);

			if(enabled){
				glEnable(GL_DEPTH_TEST);
			} else{
				glDisable(GL_DEPTH_TEST);
			}
		};

		commands.push_back(setDECommand);
	}

	void GLCommandBuffer::setClearColour(const mth::vec4f& colour){
		const auto setCCCommand = [&colour](){
			glClearColor(colour.r, colour.g, colour.b, colour.a);
		};

		commands.push_back(setCCCommand);
	}

	void GLCommandBuffer::drawIndexed(const uint32 count){
		const auto drawCommand = [count](){
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, nullptr);
		};

		commands.push_back(drawCommand);
	}

	void GLCommandBuffer::flushCommands(){
		for(auto& command : commands){
			command();
		}

		commands.clear();
	}
}
