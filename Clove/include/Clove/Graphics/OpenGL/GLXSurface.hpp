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
		std::shared_ptr<GraphicsFactory> factory;

		Display* display = nullptr;
		::Window* window;

		XVisualInfo* visual = nullptr;

		::GLXContext context = nullptr;

		PFNGLXSWAPINTERVALEXTPROC glxSwapIntervalEXT = nullptr;

		std::shared_ptr<GLRenderTarget> renderTarget;
		
		//FUNCTIONS
	public:
		GLXSurface() = delete;
		GLXSurface(std::shared_ptr<GraphicsFactory> factory, void* windowData);

		GLXSurface(const GLXSurface& other) = delete;
		GLXSurface(GLXSurface&& other) noexcept;

		GLXSurface& operator=(const GLXSurface& other) = delete;
		GLXSurface& operator=(GLXSurface&& other) noexcept;

		virtual ~GLXSurface();

		virtual const std::shared_ptr<GraphicsFactory>& getFactory() const override;

		virtual void makeCurrent() override;

		virtual void setVSync(bool enabled) override;
		virtual bool isVsync() const override;

		virtual void present() override;

		virtual std::shared_ptr<RenderTarget> getRenderTarget() const override;
	};
}