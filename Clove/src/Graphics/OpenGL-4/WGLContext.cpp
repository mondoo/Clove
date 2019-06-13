#include "clvpch.hpp"
#include "WGLContext.hpp"

#include "Platform/Windows/WindowsWindow.hpp"
#include "Platform/Windows/WindowsException.hpp"
#include "Clove/Graphics/GraphicsTypes.hpp"

#include <glad/glad.h>
#include <wglext.h>

namespace clv::gfx{
	WGLContext::WGLContext(WGLContext&& other) noexcept = default;

	WGLContext& WGLContext::operator=(WGLContext&& other) noexcept = default;

	WGLContext::~WGLContext(){
		ReleaseDC(windowsHandle, windowsDeviceContext);
		wglDeleteContext(wglContext);
	}

	WGLContext::WGLContext(void* windowData){
		windowsHandle = reinterpret_cast<WindowsData*>(windowData)->handle;

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

			CLV_LOG_DEBUG("Succesfully created an OpenGL 4.6 context");
		} else{
			CLV_LOG_WARN("Could not retrieve wglCreateContextAttribsARB. Application might not support OpenGL 3.2+ contexts");

			wglContext = wglCreateContext(windowsDeviceContext);
		}
	}

	void WGLContext::makeCurrent(){
		wglMakeCurrent(windowsDeviceContext, wglContext);
	}

	API WGLContext::getAPI() const{
		return API::OpenGL4;
	}

	void WGLContext::present(){
		SwapBuffers(windowsDeviceContext);
	}
}