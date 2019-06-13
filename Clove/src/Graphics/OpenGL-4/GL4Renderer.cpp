#include "clvpch.hpp"
#include "GL4Renderer.hpp"

#include "Clove/Platform/Window.hpp"
#include "Graphics/OpenGL-4/GL4Exception.hpp"
#include "Clove/Profiling/Timer.hpp"

#include <glad/glad.h>

namespace clv::gfx{
	GL4Renderer::~GL4Renderer() = default;

	GL4Renderer::GL4Renderer(const Context& context){
		CLV_LOG_DEBUG("Device context created");

		CLV_ASSERT(gladLoadGL(), "Failed to load OpenGL functions");

		CLV_LOG_INFO("Rendering environment created successfully!");
		CLV_LOG_INFO("GL version: {0}", glGetString(GL_VERSION));

		CLV_LOG_TRACE("Enabling Depth buffer");
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);

		CLV_LOG_TRACE("Blend set to: SRC_ALPHA | ONE_MINUS_SRC_ALPHA");
		//src is from the image - dest is what is already in the buffer
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		//I guess it's called blending because you blend the src with the destination

		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		glFrontFace(GL_CCW);

		glDebugMessageCallback(errorCallback, nullptr);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}

	void GL4Renderer::clear(){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GL4Renderer::drawIndexed(const unsigned int count){
		CLV_TIME_SCOPE("OpenGL4: DrawIndexed");
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, nullptr);
	}
}