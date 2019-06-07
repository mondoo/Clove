#include "clvpch.hpp"
#include "Component.hpp"

namespace clv::ecs{
	unsigned int Component::nextID = 0;

	Component::Component() = default;

	Component::Component(Component&& other) noexcept = default;

	Component& Component::operator=(Component&& other) noexcept = default;

	Component::~Component() = default;
}