#pragma once

#include "Clove/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	class Drawable;
	class Context;

	class Renderer{
		//VARIABLES
	protected:
		static constexpr math::Vector4f clearColor = { 1.01f, 0.5f, 0.5f, 1.0f };

		//FUNCTIONS
	public:
		Renderer();
		Renderer(const Renderer& other) = delete;
		Renderer(Renderer&& other) noexcept = delete;
		Renderer& operator=(const Renderer& other) = delete;
		Renderer& operator=(Renderer&& other) noexcept = delete;
		virtual ~Renderer();

		static std::shared_ptr<Renderer> createRenderer(const Context& context);

		virtual void clear() = 0;
		virtual void drawIndexed(const unsigned int count) = 0;
	};
}