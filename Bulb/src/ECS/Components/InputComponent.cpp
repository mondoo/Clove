#include "Bulb/ECS/Components/InputComponent.hpp"

namespace blb::ecs {
	InputComponent::InputComponent() = default;

	InputComponent::InputComponent(const InputComponent& other) = default;

	InputComponent::InputComponent(InputComponent&& other) noexcept {
		keyBindings			= std::move(other.keyBindings);
		mouseButtonBindings = std::move(other.mouseButtonBindings);
	}

	InputComponent& InputComponent::operator=(const InputComponent& other) = default;

	InputComponent& InputComponent::operator=(InputComponent&& other) noexcept{
		keyBindings			= std::move(other.keyBindings);
		mouseButtonBindings = std::move(other.mouseButtonBindings);

		return *this;
	}

	InputComponent::~InputComponent() = default;
}