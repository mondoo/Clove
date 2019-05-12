#include "clvpch.hpp"
#include "Drawable.hpp"

#include "Clove/Graphics/Bindable.hpp"

namespace clv{
	namespace gfx{
		Drawable::Drawable() = default;

		Drawable::Drawable(Drawable&& other) noexcept = default;

		Drawable::~Drawable() = default;

		void Drawable::addBindable(std::unique_ptr<Bindable> bindable){
			bindables.push_back(std::move(bindable));
		}

		Drawable& Drawable::operator=(Drawable&& other) noexcept = default;
	}
}