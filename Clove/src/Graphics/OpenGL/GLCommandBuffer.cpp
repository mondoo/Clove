#include "Clove/Graphics/OpenGL/GLCommandBuffer.hpp"

#include "Clove/Graphics/OpenGL/Resources/GLBuffer.hpp"
#include "Clove/Graphics/OpenGL/Resources/GLTexture.hpp"
#include "Clove/Graphics/OpenGL/GLPipelineObject.hpp"
#include "Clove/Graphics/OpenGL/GLRenderTarget.hpp"
#include "Clove/Graphics/OpenGL/GLShader.hpp"
#include "Clove/Graphics/OpenGL/GLSurface.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	GLCommandBuffer::GLCommandBuffer(std::shared_ptr<GraphicsFactory> factory)
		: factory(std::move(factory)) {
	}

	GLCommandBuffer::GLCommandBuffer(GLCommandBuffer&& other) noexcept = default;

	GLCommandBuffer& GLCommandBuffer::operator=(GLCommandBuffer&& other) noexcept = default;

	GLCommandBuffer::~GLCommandBuffer() = default;

	const std::shared_ptr<GraphicsFactory>& GLCommandBuffer::getFactory() const {
		return factory;
	}

	void GLCommandBuffer::beginEncoding(const std::shared_ptr<RenderTarget>& renderTarget){
		glRenderTarget = std::static_pointer_cast<GLRenderTarget>(renderTarget);

		commands.emplace_back([glRenderTarget = glRenderTarget.get()](){
			glBindFramebuffer(GL_FRAMEBUFFER, glRenderTarget->getGLFrameBufferID());
		});
	}

	void GLCommandBuffer::clearTarget(){
		commands.emplace_back([glRenderTarget = glRenderTarget.get()](){
			const auto& clearColour = glRenderTarget->getClearColour();

			glClearColor(clearColour.r, clearColour.g, clearColour.b, clearColour.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		});
	}

	void GLCommandBuffer::updateBufferData(Buffer& buffer, const void* data){
		const size_t bufferSize = buffer.getDescriptor().bufferSize;
		void* datacopy = new char[bufferSize];
		memcpy(datacopy, data, buffer.getDescriptor().bufferSize);

		commands.emplace_back([&buffer, data = datacopy](){
			GLBuffer& glbuffer = static_cast<GLBuffer&>(buffer);

			glBindBuffer(GL_UNIFORM_BUFFER, glbuffer.getBufferID());
			glBufferSubData(GL_UNIFORM_BUFFER, 0, glbuffer.getDescriptor().bufferSize, data);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			delete data;
		});
	}

	void GLCommandBuffer::bindIndexBuffer(const Buffer& buffer){
		commands.emplace_back([&buffer](){
			const GLBuffer& glbuffer = static_cast<const GLBuffer&>(buffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glbuffer.getBufferID());
		});
	}

	void GLCommandBuffer::bindVertexBuffer(const Buffer& buffer, const uint32_t stride){
		commands.emplace_back([&buffer, stride](){
			const GLBuffer& glbuffer = static_cast<const GLBuffer&>(buffer);
			glBindVertexBuffer(0, glbuffer.getBufferID(), 0, stride);
		});
	}

	void GLCommandBuffer::bindShaderResourceBuffer(const Buffer& buffer, const ShaderStage shaderType, const uint32_t bindingPoint){
		commands.emplace_back([&buffer, shaderType, bindingPoint](){
			const GLBuffer& glbuffer = static_cast<const GLBuffer&>(buffer);
			glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, glbuffer.getBufferID());
		});
	}

	void GLCommandBuffer::bindPipelineObject(const PipelineObject& pipelineObject){
		commands.emplace_back([&pipelineObject](){
			const GLPipelineObject& glPipelineObject = static_cast<const GLPipelineObject&>(pipelineObject);
			glBindVertexArray(glPipelineObject.getGLVertexArrayID());
			glUseProgram(glPipelineObject.getGLPorgramID());

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			if (glPipelineObject.isBlendEnabled()){
				glEnable(GL_BLEND);
			} else{
				glDisable(GL_BLEND);
			}

			glFrontFace(glPipelineObject.isFrontFaceCounterClockwise() ? GL_CCW : GL_CW);
			glCullFace(glPipelineObject.getCullFace() == CullFace::Back ? GL_BACK : GL_FRONT);
			glEnable(GL_CULL_FACE);
		});
	}

	void GLCommandBuffer::bindTexture(const Texture* texture, const uint32_t bindingPoint){
		commands.emplace_back([texture, bindingPoint](){
			if (const GLTexture* glTexture = static_cast<const GLTexture*>(texture)){
				glBindTextureUnit(bindingPoint, glTexture->getTextureID());
			} else{
				glBindTextureUnit(bindingPoint, 0);
			}
		});
	}

	void GLCommandBuffer::setViewport(const Viewport& viewport){
		commands.emplace_back([viewport](){
			glViewport(static_cast<GLint>(viewport.x), static_cast<GLint>(viewport.y), static_cast<GLsizei>(viewport.width), static_cast<GLsizei>(viewport.height));
		});
	}

	void GLCommandBuffer::setDepthEnabled(bool enabled){
		commands.emplace_back([enabled](){
			glDepthFunc(GL_LESS);

			if (enabled){
				glEnable(GL_DEPTH_TEST);
			} else{
				glDisable(GL_DEPTH_TEST);
			}
		});
	}

	void GLCommandBuffer::drawIndexed(const uint32_t count){
		commands.emplace_back([count](){
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, nullptr);
		});
	}

	void GLCommandBuffer::endEncoding(){
		for(auto& command : commands){
			command();
		}

		commands.clear();
		glRenderTarget.reset();
	}
}
