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

		CLV_LOG_DEBUG("Succesfully created an OpenGL context");

		*data->visual = visual;
	}

	void GLXContext::makeCurrent(){
		CLV_LOG_TRACE("Making context current");
		glXMakeCurrent(display, *window, context);

		if(!glxSwapIntervalEXT){
			const char* extensions = (char*)glXQueryExtensionsString(display, visual->screen);
			if(strstr(extensions, "GLX_EXT_swap_control") != 0){
				glxSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddress((GLubyte*)"glxSwapIntervalEXT");
			} else{
				CLV_LOG_ERROR("Could not find the GLX_EXT_swap_control. Cannot enable / disable vsync");
			}
		}
	}

	void GLXContext::setVSync(bool enabled){
		if(glxSwapIntervalEXT){
			GLXDrawable drawable = glXGetCurrentDrawable();
	
			const int32 interval = enabled ? 1 : 0;
			glxSwapIntervalEXT(display, drawable, interval);
	
			CLV_LOG_TRACE("Swap interval for GLX was set to: {0}", interval);
		}else{
			CLV_LOG_ERROR("Could not set swap interval. glxSwapIntervalEXT is unitialised. Please make sure this context is current");
		}
	}

	bool GLXContext::isVsync() const{
		if(glxSwapIntervalEXT){
			GLXDrawable drawable = glXGetCurrentDrawable();

			uint32 interval = 0;
			glXQueryDrawable(display, drawable, GLX_SWAP_INTERVAL_EXT, &interval);
		
			return (interval > 0);
		}else{
			CLV_LOG_ERROR("glxSwapIntervalEXT is unitialised. Could not retrieve swap interval. Please make sure this context is current");
			return false;
		}
	}

	API GLXContext::getAPI() const{
		return API::OpenGL4;
	}

	void GLXContext::present(){
		glXSwapBuffers(display, *window);
	}
}