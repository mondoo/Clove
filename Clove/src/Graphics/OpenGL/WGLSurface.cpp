#include "Clove/Graphics/OpenGL/WGLSurface.hpp"

#include "Clove/Platform/Windows/WindowsWindow.hpp"
#include "Clove/Platform/Windows/WindowsException.hpp"
#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/OpenGL/GLException.hpp"
#include "Clove/Graphics/OpenGL/GLRenderTarget.hpp"

namespace clv::gfx::ogl{
	WGLSurface::WGLSurface(std::shared_ptr<GraphicsFactory>  factory, void* windowData) 
		: factory(std::move(factory)) {
		#if 0
		windowsHandle = reinterpret_cast<plt::WindowsData*>(windowData)->handle;

		windowsDeviceContext = GetDC(windowsHandle);

		PIXELFORMATDESCRIPTOR pfd{};
		pfd.nSize			= sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion		= 1;
		pfd.dwFlags			= PFD_DOUBLEBUFFER;
		pfd.iPixelType		= PFD_TYPE_RGBA;
		pfd.cColorBits		= 32;
		pfd.cAlphaBits		= 8;
		pfd.cDepthBits		= 24;
		pfd.cStencilBits	= 8;

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

			GARLIC_LOG(garlicLogContext, Log::Level::Debug, "Succesfully created an OpenGL {0}.{1} context", major, minor);
		} else{
			GARLIC_LOG(garlicLogContext, Log::Level::Warning, "Could not retrieve wglCreateContextAttribsARB. Application might not support OpenGL 3.2+ contexts");

			wglContext = wglCreateContext(windowsDeviceContext);
		}
		#endif
	}

	WGLSurface::WGLSurface(WGLSurface&& other) noexcept = default;

	WGLSurface& WGLSurface::operator=(WGLSurface&& other) noexcept = default;

	WGLSurface::~WGLSurface(){
		ReleaseDC(windowsHandle, windowsDeviceContext);
		wglDeleteContext(wglContext);
	}

	const std::shared_ptr<GraphicsFactory>& WGLSurface::getFactory() const {
		return factory;
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
				GARLIC_LOG(garlicLogContext, Log::Level::Error, "Could not find the WGL_EXT_swap_control extension");
			}
		}

		CLV_ASSERT(gladLoadGL(), "Failed to load OpenGL functions");

		GARLIC_LOG(garlicLogContext, Log::Level::Trace, "GL version: {0}", glGetString(GL_VERSION));
		GARLIC_LOG(garlicLogContext, Log::Level::Trace, "GLSL version: {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));

		glDebugMessageCallback(errorCallback, nullptr);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		renderTarget = std::make_shared<GLRenderTarget>(factory);
	}

	void WGLSurface::setVSync(bool enabled){
		if(wglSwapIntervalEXT){
			const int32_t interval = enabled ? 1 : 0;
			wglSwapIntervalEXT(interval);

			GARLIC_LOG(garlicLogContext, Log::Level::Trace, "Swap interval for WGL was set to: {0}", interval);
		} else{
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "Could not set swap interval. wglSwapIntervalEXT is unitialised. Please make sure this context is current");
		}
	}

	bool WGLSurface::isVsync() const{
		if(wglGetSwapIntervalEXT){
			const uint32_t interval = wglGetSwapIntervalEXT();
			return (interval > 0);
		} else{
			GARLIC_LOG(garlicLogContext, Log::Level::Error, "wglGetSwapIntervalEXT is unitialised. Could not retrieve swap interval. Please make sure this context is current");
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