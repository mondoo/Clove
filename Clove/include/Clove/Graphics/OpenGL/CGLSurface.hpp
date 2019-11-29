#pragma once

#include "Clove/Graphics/OpenGL/GLSurface.hpp"

#include <OpenGL/OpenGL.h>

namespace clv::gfx::ogl{
	class CGLSurface : public GLSurface{
		//VARIABLES
	private:
		CGLContextObj contextObject = nullptr;
		CGLPixelFormatObj pixelFormatObject = nullptr;
		
		GLint vsyncInt = 0;
		
		//FUNCTIONS
	public:
		CGLSurface() = delete;
		CGLSurface(void* windowData);
		CGLSurface(const CGLSurface& other) = delete;
		CGLSurface(CGLSurface&& other) noexcept;
		CGLSurface& operator=(const CGLSurface& other) = delete;
		CGLSurface& operator=(CGLSurface&& other) noexcept;
		virtual ~CGLSurface();

		virtual void makeCurrent() override;

		virtual void setVSync(bool vsync) override;
		virtual bool isVsync() const override;

		virtual void present() override;
	};
}
