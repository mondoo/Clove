#include "clvpch.hpp"
#include "GL4RenderAPI.hpp"

#include "Graphics/OpenGL-4/GL4Exception.hpp"
#include "Graphics/OpenGL-4/Bindables/GL4IndexBuffer.hpp"

#include <glad/glad.h>

namespace clv::gfx{
	GL4RenderAPI::~GL4RenderAPI() = default;

	GL4RenderAPI::GL4RenderAPI(const Context& context){
		CLV_ASSERT(gladLoadGL(), "Failed to load OpenGL functions");

		CLV_LOG_TRACE("GL version: {0}", glGetString(GL_VERSION));

		CLV_LOG_TRACE("Enabling Depth buffer");
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);

		CLV_LOG_TRACE("Blend set to: SRC_ALPHA | ONE_MINUS_SRC_ALPHA");
		//src is from the image - dest is what is already in the buffer
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		//I guess it's called blending because you blend the src with the destination

		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		glDebugMessageCallback(errorCallback, nullptr);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}

	void GL4RenderAPI::clear(){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GL4RenderAPI::drawIndexed(const unsigned int count){
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, nullptr);
	}

	void GL4RenderAPI::setClearColour(const math::Vector4f& colour){
		glClearColor(colour.r, colour.g, colour.b, colour.a);
	}

	void GL4RenderAPI::setIndexBuffer(const IndexBuffer& buffer){
		const GL4IndexBuffer& glBuff = static_cast<const GL4IndexBuffer&>(buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBuff.getBufferID());
	}
}