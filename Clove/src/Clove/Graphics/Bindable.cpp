#include "clvpch.hpp"
#include "Bindable.hpp"

namespace clv::gfx{
	Bindable::Bindable() = default;

	Bindable::Bindable(Bindable&& other) noexcept = default;

	Bindable& Bindable::operator=(Bindable&& other) noexcept = default;

	Bindable::~Bindable() = default;
}