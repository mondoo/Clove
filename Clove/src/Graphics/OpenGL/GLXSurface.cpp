#include "Clove/Graphics/OpenGL/GLXSurface.hpp"

#include "Clove/Platform/Linux/LinuxWindow.hpp"
#include "Clove/Graphics/Core/GraphicsTypes.hpp"
#include "Clove/Graphics/OpenGL/GLException.hpp"
#include "Clove/Graphics/OpenGL/GLRenderTarget.hpp"

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

namespace clv::gfx::ogl{
	GLXSurface::GLXSurface(void* windowData){
		plt::LinuxData* data = reinterpret_cast<plt::LinuxData*>(windowData);
		display = data->display;
		window = data->window;

		int visualAttribs[] = {
			GLX_X_RENDERABLE, True,
			GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
			GLX_RENDER_TYPE, GLX_RGBA_BIT,
			GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
			GLX_RED_SIZE, 8,
			GLX_GREEN_SIZE, 8,
			GLX_BLUE_SIZE, 8,
			GLX_ALPHA_SIZE, 8,
			GLX_DEPTH_SIZE, 24,
			GLX_STENCIL_SIZE, 8,
			GLX_DOUBLEBUFFER, True,
			0
		};

		int fbCount;
		GLXFBConfig* fbc = glXChooseFBConfig(display, DefaultScreen(display), visualAttribs, &fbCount);

		//Choose the best frame buffer config
		int bestFBC = -1;
		int bestNumSamp = -1;
		for (int i = 0; i < fbCount; ++i){
			if (XVisualInfo *vi = glXGetVisualFromFBConfig(display, fbc[i])){
				int samp_buf;
				int samples;
				glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
				glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLES, &samples);

				if (bestFBC < 0 || samp_buf && samples > bestNumSamp){
					bestFBC = i;
					bestNumSamp = samples;
				}

				XFree(vi);
			}
		}

		GLXFBConfig bestFbc = fbc[bestFBC];

		XFree(fbc);

		visual = glXGetVisualFromFBConfig( display, bestFbc );
		*data->visual = visual;

		glXCreateContextAttribsARBProc glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte *) "glXCreateContextAttribsARB");
		if(glXCreateContextAttribsARB){
			int32_t major = 4;
			int32_t minor = 6;

			int32_t attributes[] = {
				GLX_CONTEXT_MAJOR_VERSION_ARB, major,
				GLX_CONTEXT_MINOR_VERSION_ARB, minor,
				GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
				0
			};

			context = glXCreateContextAttribsARB(display, bestFbc, 0, GL_TRUE, attributes);

			CLV_LOG_DEBUG("Succesfully created an OpenGL {0}.{1} context", major, minor);
		}else{
			CLV_LOG_WARN("Could not retrieve glXCreateContextAttribsARBProc. Application might not support OpenGL3.2+ contexts");

			context = glXCreateContext(display, visual, nullptr, GL_TRUE);
		}
	}

	GLXSurface::GLXSurface(GLXSurface&& other) noexcept = default;

	GLXSurface& GLXSurface::operator=(GLXSurface&& other) noexcept = default;

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
	
			const int32_t interval = enabled ? 1 : 0;
			glxSwapIntervalEXT(display, drawable, interval);
	
			CLV_LOG_TRACE("Swap interval for GLX was set to: {0}", interval);
		}else{
			CLV_LOG_ERROR("Could not set swap interval. glxSwapIntervalEXT is unitialised. Please make sure this context is current");
		}
	}

	bool GLXSurface::isVsync() const{
		if(glxSwapIntervalEXT){
			GLXDrawable drawable = glXGetCurrentDrawable();

			uint32_t interval = 0;
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

	std::shared_ptr<RenderTarget> GLXSurface::getRenderTarget() const{
		return renderTarget;
	}
}