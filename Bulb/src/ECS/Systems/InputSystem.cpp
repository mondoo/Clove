#include "Bulb/ECS/Systems/InputSystem.hpp"

#include <Clove/Platform/Window.hpp>

namespace blb::ecs {
	InputSystem::InputSystem(std::shared_ptr<clv::plt::Window> window) 
		: window(std::move(window)){
	}

	InputSystem::InputSystem(const InputSystem& other) = default;

	InputSystem::InputSystem(InputSystem&& other) noexcept = default;

	InputSystem& InputSystem::operator=(const InputSystem& other) = default;

	InputSystem& InputSystem::operator=(InputSystem&& other) noexcept = default;

	InputSystem::~InputSystem() = default;

	void InputSystem::update(World& world, clv::utl::DeltaTime deltaTime) {

	}
}