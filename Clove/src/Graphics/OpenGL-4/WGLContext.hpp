#pragma once

#include "Clove/Graphics/Context.hpp"

namespace clv::gfx{
	class WGLContext : public Context{
		//VARIABLES
	private:
		HWND windowsHandle = nullptr;
		HDC windowsDeviceContext = nullptr;
		HGLRC wglContext = nullptr;

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

		virtual void present() override;
	};
}
