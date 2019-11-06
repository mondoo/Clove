#pragma once

#include "Core/Graphics/Context.hpp"

#include <glad/glad.h>
#include <wglext.h>

namespace clv::gfx{
	class WGLContext : public Context{
		//VARIABLES
	private:
		HWND windowsHandle = nullptr;
		HDC windowsDeviceContext = nullptr;
		HGLRC wglContext = nullptr;

		PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;
		PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = nullptr;

		//FUNCTIONS
	public:
		WGLContext() = delete;
		WGLContext(const WGLContext& other) = delete;
		WGLContext(WGLContext&& other) noexcept;
		WGLContext& operator=(const WGLContext& other) = delete;
		WGLContext& operator=(WGLContext&& other) noexcept;
		virtual ~WGLContext();

		WGLContext(void* windowData);

		virtual void makeCurrent() override;

		virtual void setVSync(bool enabled) override;
		virtual bool isVsync() const override;

		virtual API getAPI() const override;

		virtual void present() override;
	};
}
