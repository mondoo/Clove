#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv{
	class Window;
}

namespace clv::gfx{
	class Drawable;

	class Renderer{
		//FUNCTIONS
	public:
		Renderer();
		Renderer(const Renderer& other) = delete;
		Renderer(Renderer&& other) noexcept = delete;
		Renderer& operator=(const Renderer& other) = delete;
		Renderer& operator=(Renderer&& other) noexcept = delete;
		virtual ~Renderer();

		static std::unique_ptr<Renderer> createRenderer(const Window& window, API api);

		virtual void clear() = 0;
		virtual void drawIndexed(const unsigned int count) = 0;
		virtual void swapBuffers() = 0;

		inline virtual API getAPI() const = 0;
	};
}