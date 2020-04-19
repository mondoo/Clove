#pragma once

#include "Clove/Graphics/OpenGL/GLSurface.hpp"

#include <glad/glad.h>
#include <wglext.h>

namespace clv::gfx::ogl{
	class GLRenderTarget;
}

namespace clv::gfx::ogl{
	class WGLSurface : public GLSurface{
		//VARIABLES
	private:
		std::shared_ptr<GraphicsFactory> factory;

		HWND windowsHandle = nullptr;
		HDC windowsDeviceContext = nullptr;
		HGLRC wglContext = nullptr;

		PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;
		PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = nullptr;

		std::shared_ptr<GLRenderTarget> renderTarget;

		//FUNCTIONS
	public:
		WGLSurface() = delete;
		WGLSurface(std::shared_ptr<GraphicsFactory> factory, void* windowData);

		WGLSurface(const WGLSurface& other) = delete;
		WGLSurface(WGLSurface&& other) noexcept;

		WGLSurface& operator=(const WGLSurface& other) = delete;
		WGLSurface& operator=(WGLSurface&& other) noexcept;

		virtual ~WGLSurface();

		virtual const std::shared_ptr<GraphicsFactory>& getFactory() const override;

		virtual void makeCurrent() override;

		virtual void setVSync(bool enabled) override;
		virtual bool isVsync() const override;

		virtual void present() override;

		virtual std::shared_ptr<RenderTarget> getRenderTarget() const override;
	};
}
