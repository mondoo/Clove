#include "Component.hpp"

namespace clv::ecs{
	Component::Component() = default;

	Component::Component(Component&& other) noexcept = default;

	Component& Component::operator=(Component&& other) noexcept = default;

	Component::~Component() = default;
}