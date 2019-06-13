#include "clvpch.hpp"
#include "GLXContext.hpp"

#include "Platform/Linux/LinuxWindow.hpp"
#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	GLXContext::GLXContext(GLXContext&& other) = default;

	GLXContext& GLXContext::operator=(GLXContext&& other) = default;

	GLXContext::~GLXContext(){
		glXDestroyContext(display, context);
	}

	GLXContext::GLXContext(void* windowData){
		LinuxData* data = reinterpret_cast<LinuxData*>(windowData);
		display = data->display;
		window = data->window;

		GLint glxAttribs[] = {
			GLX_RGBA,
			GLX_DEPTH_SIZE, 24,
			GLX_DOUBLEBUFFER,
			0
		};

		visual = glXChooseVisual(display, 0, glxAttribs);
		if(!visual){
			//TODO: Exception
			CLV_LOG_CRITICAL("Could not create visual");
			return;
		}

		context = glXCreateContext(display, visual, nullptr, GL_TRUE);
		if(!context){
			//TODO:Exception
			CLV_LOG_CRITICAL("Could not create context");
			return;
		}

		*data->visual = visual;
	}

	void GLXContext::makeCurrent(){
		CLV_LOG_TRACE("Making context current");
		glXMakeCurrent(display, *window, context);
	}

	API GLXContext::getAPI() const{
		return API::OpenGL4;
	}

	void GLXContext::present(){
		glXSwapBuffers(display, *window);
	}
}