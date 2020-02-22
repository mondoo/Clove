#include "Clove/Graphics/OpenGL/WGLSurface.hpp"

#include "Clove/Platform/Windows/WindowsWindow.hpp"
#include "Clove/Platform/Windows/WindowsException.hpp"
#include "Clove/Graphics/Core/GraphicsTypes.hpp"
#include "Clove/Graphics/OpenGL/GLException.hpp"
#include "Clove/Graphics/OpenGL/GLRenderTarget.hpp"

namespace clv::gfx::ogl{
	WGLSurface::WGLSurface(void* windowData){
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

		int32_t pf;
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
		if(wglCreateContextAttribsARB != nullptr){
			int32_t major = 4;
			int32_t minor = 6;

			int32_t attributes[] = {
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

	WGLSurface::WGLSurface(WGLSurface&& other) noexcept = default;

	WGLSurface& WGLSurface::operator=(WGLSurface&& other) noexcept = default;

	WGLSurface::~WGLSurface(){
		ReleaseDC(windowsHandle, windowsDeviceContext);
		wglDeleteContext(wglContext);
	}

	void WGLSurface::makeCurrent(){
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

		CLV_ASSERT(gladLoadGL(), "Failed to load OpenGL functions");

		CLV_LOG_TRACE("GL version: {0}", glGetString(GL_VERSION));
		CLV_LOG_TRACE("GLSL version: {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));

		glDebugMessageCallback(errorCallback, nullptr);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		renderTarget = std::make_shared<GLRenderTarget>();
	}

	void WGLSurface::setVSync(bool enabled){
		if(wglSwapIntervalEXT){
			const int32_t interval = enabled ? 1 : 0;
			wglSwapIntervalEXT(interval);

			CLV_LOG_TRACE("Swap interval for WGL was set to: {0}", interval);
		} else{
			CLV_LOG_ERROR("Could not set swap interval. wglSwapIntervalEXT is unitialised. Please make sure this context is current");
		}
	}

	bool WGLSurface::isVsync() const{
		if(wglGetSwapIntervalEXT){
			const uint32_t interval = wglGetSwapIntervalEXT();
			return (interval > 0);
		} else{
			CLV_LOG_ERROR("wglGetSwapIntervalEXT is unitialised. Could not retrieve swap interval. Please make sure this context is current");
			return false;
		}
	}

	void WGLSurface::present(){
		SwapBuffers(windowsDeviceContext);
	}

	std::shared_ptr<RenderTarget> WGLSurface::getRenderTarget() const{
		return renderTarget;
	}
}