#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	class Drawable;
	class Context;

	class Renderer{
		//FUNCTIONS
	public:
		Renderer();
		Renderer(const Renderer& other) = delete;
		Renderer(Renderer&& other) noexcept = delete;
		Renderer& operator=(const Renderer& other) = delete;
		Renderer& operator=(Renderer&& other) noexcept = delete;
		virtual ~Renderer();

		static std::unique_ptr<Renderer> createRenderer(const Context& context, API api);

		virtual void clear() = 0;
		virtual void drawIndexed(const unsigned int count) = 0;
		inline virtual API getAPI() const = 0;
	};
}