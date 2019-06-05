#include "clvpch.hpp"
#include "GL4Renderer.hpp"

#include "Clove/Platform/Window.hpp"
#include "Graphics/OpenGL-4/GL4Exception.hpp"
#include "Clove/Profiling/Timer.hpp"
#if CLV_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsException.hpp"
#elif CLV_PLATFORM_LINUX
#include "Platform/Linux/XLibHelpers.hpp"
#endif

#if CLV_PLATFORM_WINDOWS
#include <glad/glad.h>
#include <wglext.h>
#endif

namespace clv::gfx{
	GL4Renderer::~GL4Renderer(){
	#if CLV_PLATFORM_WINDOWS
		ReleaseDC(windowsHandle, windowsDeviceContext);
		wglDeleteContext(wglContext);
	#elif CLV_PLATFORM_LINUX
		glXDestroyContext(xDisplay, glxContext);
	#endif
	}

	GL4Renderer::GL4Renderer(const Window& window){
	#if CLV_PLATFORM_WINDOWS
		windowsHandle = reinterpret_cast<HWND>(window.getNativeWindow());

		windowsDeviceContext = GetDC(windowsHandle);

		PIXELFORMATDESCRIPTOR pfd = { 0 };
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cAlphaBits = 8;
		pfd.cDepthBits = 24;

		int pf;
		pf = ChoosePixelFormat(windowsDeviceContext, &pfd);
		if(pf == 0){
			throw CLV_WINDOWS_LAST_EXCEPTION;
		}

		if(SetPixelFormat(windowsDeviceContext, pf, &pfd) == FALSE){
			throw CLV_WINDOWS_LAST_EXCEPTION;
		}

		//Create a temp context to get the proecAdresss for wglCreateContextAttribsARB
		HGLRC tempOpenGLContext = wglCreateContext(windowsDeviceContext);
		wglMakeCurrent(windowsDeviceContext, tempOpenGLContext);

		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		if(wglCreateContextAttribsARB){
			int attributes[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
				WGL_CONTEXT_MINOR_VERSION_ARB, 6,
				WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
				0
			};

			//Create the new 4.6 context
			wglContext = wglCreateContextAttribsARB(windowsDeviceContext, NULL, attributes);

			//Remove the old temp one
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(tempOpenGLContext);

			//Swap in the fancy new one
			wglMakeCurrent(windowsDeviceContext, wglContext);

			CLV_LOG_INFO("Succesfully created an OpenGL 4.6 context");
		} else{
			CLV_LOG_WARN("Could not retrieve wglCreateContextAttribsARB. Application might not support OpenGL 3.2+ contexts");

			wglContext = wglCreateContext(windowsDeviceContext);
			wglMakeCurrent(windowsDeviceContext, wglContext);
		}
	#elif CLV_PLATFORM_LINUX
		Screen* xScreen = reinterpret_cast<Screen*>(window.getNativeWindow());
		xDisplay = xScreen->display;
		xWindow = xScreen->root; //Note this is the root window ID - might cause issues when having multiple windows

		XVisualInfo* visual = XLibHelpers::getVisualInfo(xDisplay, API::OpenGL4);
        if(!visual){
            //TODO: Exception
            CLV_LOG_CRITICAL("Could not get VisualInfo");
        }

		glxContext = glXCreateContext(xDisplay, visual, nullptr, GL_TRUE);
        
        if(!glxContext){
            //TODO:Exception
            CLV_LOG_CRITICAL("Could not create context");
            return;
        }
	
        CLV_LOG_TRACE("Making context current");
        glXMakeCurrent(xDisplay, xWindow, glxContext);
	#endif

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

	API GL4Renderer::getAPI() const{
		return API::OpenGL4;
	}

	void GL4Renderer::drawIndexed(const unsigned int count){
		CLV_TIME_SCOPE("OpenGL4: DrawIndexed");

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, nullptr);
	}

	void GL4Renderer::swapBuffers(){
	#if CLV_PLATFORM_WINDOWS
		SwapBuffers(windowsDeviceContext);
	#elif CLV_PLATFORM_LINUX
		glXSwapBuffers(xDisplay, xWindow);
	#endif
	}
}