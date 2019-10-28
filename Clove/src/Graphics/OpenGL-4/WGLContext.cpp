#include "WGLContext.hpp"

#include "Platform/Windows/WindowsWindow.hpp"
#include "Platform/Windows/WindowsException.hpp"
#include "Core/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	WGLContext::WGLContext(WGLContext&& other) noexcept = default;

	WGLContext& WGLContext::operator=(WGLContext&& other) noexcept = default;

	WGLContext::~WGLContext(){
		ReleaseDC(windowsHandle, windowsDeviceContext);
		wglDeleteContext(wglContext);
	}

	WGLContext::WGLContext(void* windowData){
		windowsHandle = reinterpret_cast<plt::WindowsData*>(windowData)->handle;

		windowsDeviceContext = GetDC(windowsHandle);

		PIXELFORMATDESCRIPTOR pfd = { 0 };
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cAlphaBits = 8;
		pfd.cDepthBits = 24;

		int32 pf;
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
			int32 major = 4;
			int32 minor = 1;
			
			int32 attributes[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, major,
				WGL_CONTEXT_MINOR_VERSION_ARB, minor,
				WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
				0
			};

			//Create the new 4.6 context
			wglContext = wglCreateContextAttribsARB(windowsDeviceContext, NULL, attributes);

			//Remove the old temp one
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(tempOpenGLContext);

			CLV_LOG_DEBUG("Succesfully created an OpenGL {0}.{1} context", major, minor);
		} else{
			CLV_LOG_WARN("Could not retrieve wglCreateContextAttribsARB. Application might not support OpenGL 3.2+ contexts");

			wglContext = wglCreateContext(windowsDeviceContext);
		}
	}

	void WGLContext::makeCurrent(){
		wglMakeCurrent(windowsDeviceContext, wglContext);

		if(!wglSwapIntervalEXT || !wglGetSwapIntervalEXT){
			PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
			const char* extensions = wglGetExtensionsStringARB(windowsDeviceContext);
			if(strstr(extensions, "WGL_EXT_swap_control") != 0){
				wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
				wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
			} else{
				CLV_LOG_ERROR("Could not find the WGL_EXT_swap_control extension");
			}
		}
	}

	void WGLContext::setVSync(bool enabled){
		if(wglSwapIntervalEXT){
			const int32 interval = enabled ? 1 : 0;
			wglSwapIntervalEXT(interval);

			CLV_LOG_TRACE("Swap interval for WGL was set to: {0}", interval);
		} else{
			CLV_LOG_ERROR("Could not set swap interval. wglSwapIntervalEXT is unitialised. Please make sure this context is current");
		}
	}

	bool WGLContext::isVsync() const{
		if(wglGetSwapIntervalEXT){
			const uint32 interval = wglGetSwapIntervalEXT();
			return (interval > 0);
		} else{
			CLV_LOG_ERROR("wglGetSwapIntervalEXT is unitialised. Could not retrieve swap interval. Please make sure this context is current");
			return false;
		}
	}

	API WGLContext::getAPI() const{
		return API::OpenGL4;
	}

	void WGLContext::present(){
		SwapBuffers(windowsDeviceContext);
	}
}