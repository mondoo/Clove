#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

#include <queue>

namespace clv::gfx{
	class Drawable;
	class Context;

	class Renderer{
		friend class Drawable;

		//VARIABLES
	private:
		std::queue<std::weak_ptr<Drawable>> drawables;

		//FUNCTIONS
	public:
		Renderer();
		Renderer(const Renderer& other) = delete;
		Renderer(Renderer&& other) noexcept = delete;
		Renderer& operator=(const Renderer& other) = delete;
		Renderer& operator=(Renderer&& other) noexcept = delete;
		virtual ~Renderer();

		static std::unique_ptr<Renderer> createRenderer(const Context& context, API api);

		void submit(const std::shared_ptr<Drawable>& drawable);

		virtual void clear() = 0;
		void draw();

		virtual API getAPI() const = 0;

	protected:
		virtual void drawIndexed(const unsigned int count) = 0;

		virtual void swapBuffers() = 0;
	};
}