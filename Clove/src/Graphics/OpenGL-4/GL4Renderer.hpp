#pragma once

#include "Clove/Graphics/Renderer.hpp"

namespace clv::gfx{
	class GL4Renderer : public Renderer{
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
		GL4Renderer() = delete;
		GL4Renderer(const GL4Renderer& other) = delete;
		GL4Renderer(GL4Renderer&& other) noexcept = delete;
		GL4Renderer& operator=(const GL4Renderer& other) = delete;
		GL4Renderer& operator=(GL4Renderer&& other) noexcept = delete;
		~GL4Renderer();

		GL4Renderer(const Window& window);

		virtual void clear() override;

		inline virtual API getAPI() const override;

	protected:
		virtual void drawIndexed(const unsigned int count) override;

		virtual void swapBuffers() override;
	};
}

#include "GL4Renderer.inl"