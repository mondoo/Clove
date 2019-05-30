#include "clvpch.hpp"
#include "GLRenderer.hpp"

#include "Clove/Platform/Window.hpp"
#include "Graphics/OpenGL/GLException.hpp"
#include "Clove/Profiling/Timer.hpp"

#if CLV_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsException.hpp"
#endif

#include <glad/glad.h>
#include <gl/GL.h>
#include <wglext.h>

#pragma comment(lib, "opengl32.lib")

namespace clv::gfx{
	GLRenderer::~GLRenderer(){
	#if CLV_PLATFORM_WINDOWS
		ReleaseDC(windowsHandle, windowsDeviceContext);
		wglDeleteContext(windowsResourceContext);
	#endif
	}

	GLRenderer::GLRenderer(const Window& window){
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
			windowsResourceContext = wglCreateContextAttribsARB(windowsDeviceContext, NULL, attributes);

			//Remove the old temp one
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(tempOpenGLContext);

			//Swap in the fancy new one
			wglMakeCurrent(windowsDeviceContext, windowsResourceContext);

			CLV_LOG_INFO("Succesfully created an OpenGL 4.6 context");
		} else{
			CLV_LOG_WARN("Could not retrieve wglCreateContextAttribsARB. Application might not support OpenGL 3.2+ contexts");

			windowsResourceContext = wglCreateContext(windowsDeviceContext);
			wglMakeCurrent(windowsDeviceContext, windowsResourceContext);
		}

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

	void GLRenderer::clear(){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLRenderer::drawIndexed(const unsigned int count){
		CLV_TIME_SCOPE("OpenGL4: DrawIndexed");

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, nullptr);
	}

	void GLRenderer::swapBuffers(){
	#if CLV_PLATFORM_WINDOWS
		SwapBuffers(windowsDeviceContext);
	#endif
	}
}