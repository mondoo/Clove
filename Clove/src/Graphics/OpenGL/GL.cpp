#include "Clove/Graphics/OpenGL/GL.hpp"

#include "Clove/Graphics/OpenGL/GLRenderFactory.hpp"
#include "Clove/Graphics/OpenGL/GLException.hpp"

#include <glad/glad.h>

namespace clv::gfx::ogl{
	std::unique_ptr<RenderFactory> initialiseOGL(){
		return std::make_unique<GLRenderFactory>();
	}

	//Commented out for now - I think the context should load glad
	//the depth / blend can be handled properly by the context and pipeline repsectively
	//For now the culling and debugging can be put in the context

	//void postContextCreatedInitialise(){
	//	CLV_ASSERT(gladLoadGL(), "Failed to load OpenGL functions");

	//	//TODO: The below should be handled by either application or client
	//	CLV_LOG_TRACE("GL version: {0}", glGetString(GL_VERSION));
	//	CLV_LOG_TRACE("GLSL version: {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//	glDepthFunc(GL_LESS); //TODO: Expose to function call

	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //TODO: Expose to function call

	//	glFrontFace(GL_CCW);
	//	glCullFace(GL_BACK);
	//	glEnable(GL_CULL_FACE);

	//	glDebugMessageCallback(errorCallback, nullptr);
	//	glEnable(GL_DEBUG_OUTPUT);
	//	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	//}
}