#include "Clove/Graphics/OpenGL/GLXSurface.hpp"

#include "Clove/Platform/Linux/LinuxWindow.hpp"
#include "Clove/Graphics/Core/GraphicsTypes.hpp"
#include "Clove/Graphics/OpenGL/GLException.hpp"
#include "Clove/Graphics/OpenGL/GLRenderTarget.hpp"

namespace clv::gfx::ogl{
	GLXSurface::GLXSurface(void* windowData){
		plt::LinuxData* data = reinterpret_cast<plt::LinuxData*>(windowData);
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

	GLXSurface::GLXSurface(GLXSurface&& other) = default;

	GLXSurface& GLXSurface::operator=(GLXSurface&& other) = default;

	GLXSurface::~GLXSurface(){
		glXDestroyContext(display, context);
	}

	void GLXSurface::makeCurrent(){
		glXMakeCurrent(display, *window, context);

		if(!glxSwapIntervalEXT){
			const char* extensions = (char*)glXQueryExtensionsString(display, visual->screen);
			if(strstr(extensions, "GLX_EXT_swap_control") != 0){
				glxSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddress((GLubyte*)"glxSwapIntervalEXT");
			} else{
				CLV_LOG_ERROR("Could not find the GLX_EXT_swap_control. Cannot enable / disable vsync");
			}
		}

		CLV_ASSERT(gladLoadGL(), "Failed to load OpenGL functions");

		CLV_LOG_TRACE("GL version: {0}", glGetString(GL_VERSION));
		CLV_LOG_TRACE("GLSL version: {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));

		glDebugMessageCallback(errorCallback, nullptr);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		renderTarget = std::make_shared<GLRenderTarget>();
	}

	void GLXSurface::setVSync(bool enabled){
		if(glxSwapIntervalEXT){
			GLXDrawable drawable = glXGetCurrentDrawable();
	
			const int32 interval = enabled ? 1 : 0;
			glxSwapIntervalEXT(display, drawable, interval);
	
			CLV_LOG_TRACE("Swap interval for GLX was set to: {0}", interval);
		}else{
			CLV_LOG_ERROR("Could not set swap interval. glxSwapIntervalEXT is unitialised. Please make sure this context is current");
		}
	}

	bool GLXSurface::isVsync() const{
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

	void GLXSurface::present(){
		glXSwapBuffers(display, *window);
	}

	const std::shared_ptr<RenderTarget>& GLXSurface::getRenderTarget() const{
		return renderTarget;
	}
}