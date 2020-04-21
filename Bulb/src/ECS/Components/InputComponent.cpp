#include "Bulb/ECS/Components/InputComponent.hpp"

namespace blb::ecs {
	InputComponent::InputComponent() = default;

	InputComponent::InputComponent(const InputComponent& other) = default;

	InputComponent::InputComponent(InputComponent&& other) noexcept = default;

	InputComponent& InputComponent::operator=(const InputComponent& other) = default;

	InputComponent& InputComponent::operator=(InputComponent&& other) noexcept = default;

	InputComponent::~InputComponent() = default;
}