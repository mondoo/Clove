#pragma once

#include "Core/Graphics/Context.hpp"

#include <OpenGL/OpenGL.h>

namespace clv::gfx{
	class CGLContext : public Context{
		//VARIABLES
	private:
		CGLContextObj contextObject = nullptr;
		CGLPixelFormatObj pixelFormatObject = nullptr;
		
		GLint vsyncInt = 0;
		
		//FUNCTIONS
	public:
		CGLContext() = delete;
		CGLContext(const CGLContext& other) = delete;
		CGLContext(CGLContext&& other) noexcept;
		CGLContext& operator=(const CGLContext& other) = delete;
		CGLContext& operator=(CGLContext&& other) noexcept;
		virtual ~CGLContext();
		
		CGLContext(void* windowData);
		
		virtual void makeCurrent() override;

		virtual void setVSync(bool vsync) override;
		virtual bool isVsync() const override;

		virtual API getAPI() const override;

		virtual void present() override;
	};
}
