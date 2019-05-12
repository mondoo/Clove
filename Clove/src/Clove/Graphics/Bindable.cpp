#include "clvpch.hpp"
#include "Bindable.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/RendererContext.hpp"

namespace clv{
	namespace gfx{
		Bindable::Bindable() = default;

		Bindable::Bindable(Bindable&& other) noexcept = default;

		Bindable& Bindable::operator=(Bindable&& other) noexcept = default;
		
		Bindable::~Bindable() = default;
	}
}