#pragma once

#include "Graphics/OpenGL/GLSurface.hpp"

#include <glad/glad.h>
#include <wglext.h>

namespace clv::gfx::ogl{
	class WGLSurface : public GLSurface{
		//VARIABLES
	private:
		HWND windowsHandle = nullptr;
		HDC windowsDeviceContext = nullptr;
		HGLRC wglContext = nullptr;

		PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;
		PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = nullptr;

		//FUNCTIONS
	public:
		WGLSurface() = delete;
		WGLSurface(void* windowData);
		WGLSurface(const WGLSurface& other) = delete;
		WGLSurface(WGLSurface&& other) noexcept;
		WGLSurface& operator=(const WGLSurface& other) = delete;
		WGLSurface& operator=(WGLSurface&& other) noexcept;
		virtual ~WGLSurface();

		virtual void makeCurrent() override;

		virtual void setVSync(bool enabled) override;
		virtual bool isVsync() const override;

		virtual void present() override;
	};
}
