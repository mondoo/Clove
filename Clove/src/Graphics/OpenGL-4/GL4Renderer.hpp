#pragma once

#include "Clove/Graphics/Renderer.hpp"

namespace clv::gfx{
	class Context;

	class GL4Renderer : public Renderer{
		//VARIABLES
	private:
	#if CLV_PLATFORM_LINUX
		Display* linuxDisplay = nullptr;
	#endif

		//FUNCTIONS
	public:
		GL4Renderer() = delete;
		GL4Renderer(const GL4Renderer& other) = delete;
		GL4Renderer(GL4Renderer&& other) noexcept = delete;
		GL4Renderer& operator=(const GL4Renderer& other) = delete;
		GL4Renderer& operator=(GL4Renderer&& other) noexcept = delete;
		~GL4Renderer();

		GL4Renderer(const Context& context);

		virtual void clear() override;

		virtual API getAPI() const override;

	protected:
		virtual void drawIndexed(const unsigned int count) override;

		virtual void swapBuffers() override;
	};
}