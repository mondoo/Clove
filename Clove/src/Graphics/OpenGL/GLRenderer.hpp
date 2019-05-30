#pragma once

#include "Clove/Graphics/Renderer.hpp"

namespace clv::gfx{
	class GLRenderer : public Renderer{
		//VARIABLES
	private:
	#if CLV_PLATFORM_WINDOWS
		HWND windowsHandle = nullptr;
		HDC windowsDeviceContext = nullptr;
		HGLRC windowsResourceContext = nullptr;
	#elif CLV_PLATFORM_LINUX

	#endif

		//FUNCTIONS
	public:
		GLRenderer() = delete;
		GLRenderer(const GLRenderer& other) = delete;
		GLRenderer(GLRenderer&& other) noexcept = delete;
		GLRenderer& operator=(const GLRenderer& other) = delete;
		GLRenderer& operator=(GLRenderer&& other) noexcept = delete;
		~GLRenderer();

		GLRenderer(const Window& window);

		virtual void clear() override;

		inline virtual API getAPI() const override;

	protected:
		virtual void drawIndexed(const unsigned int count) override;
	};
}

#include "GLRenderer.inl"