#pragma once

#include "Clove/Graphics/OpenGL/GLSurface.hpp"

#include <glad/glad.h>
#include <GL/glx.h>
#undef None

namespace clv::gfx::ogl{
	class GLRenderTarget;
}

namespace clv::gfx::ogl{
	class GLXSurface : public GLSurface{
		//VARIABLES
	private:
		Display* display = nullptr;
		::Window* window;

		XVisualInfo* visual = nullptr;

		::GLXContext context = nullptr;

		PFNGLXSWAPINTERVALEXTPROC glxSwapIntervalEXT = nullptr;

		std::shared_ptr<GLRenderTarget> renderTarget;
		
		//FUNCTIONS
	public:
		GLXSurface() = delete;
		GLXSurface(void* windowData);

		GLXSurface(const GLXSurface& other) = delete;
		GLXSurface(GLXSurface&& other);

		GLXSurface& operator=(const GLXSurface& other) = default;
		GLXSurface& operator=(GLXSurface&& other);

		virtual ~GLXSurface();

		virtual void makeCurrent() override;

		virtual void setVSync(bool enabled) override;
		virtual bool isVsync() const override;

		virtual void present() override;

		virtual std::shared_ptr<RenderTarget> getRenderTarget() const override;
	};
}