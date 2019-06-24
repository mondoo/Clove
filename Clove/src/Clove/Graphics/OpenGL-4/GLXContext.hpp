#pragma once

#include "Clove/Graphics/Context.hpp"

#include <GL/glx.h>

namespace clv::gfx{
	class GLXContext : public Context{
		//VARIABLES
	private:
		Display* display = nullptr;
		::Window* window;

		XVisualInfo* visual = nullptr;

		::GLXContext context = nullptr;
		
		//FUNCTIONS
	public:
		GLXContext() = delete;
		GLXContext(const GLXContext& other) = delete;
		GLXContext(GLXContext&& other);
		GLXContext& operator=(const GLXContext& other) = default;
		GLXContext& operator=(GLXContext&& other);
		virtual ~GLXContext();

		GLXContext(void* windowData);

		virtual void makeCurrent() override;

		virtual void setVSync(bool enabled) override;
		virtual bool isVsync() const override;

		virtual API getAPI() const override;

		virtual void present() override;
	};
}