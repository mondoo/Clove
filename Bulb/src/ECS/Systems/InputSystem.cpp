#include "Bulb/ECS/Systems/InputSystem.hpp"

#include "Bulb/ECS/World.hpp"
#include "Bulb/ECS/Components/InputComponent.hpp"

#include <Clove/Platform/Window.hpp>

using namespace clv;

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
		auto inputSets = world.getComponentSets<InputComponent>();
		
		//Keyboard
		while(std::optional<Keyboard::Event> keyEvent = window->getKeyboard().getKeyEvent()) {
			for(auto&& [inputComp] : inputSets) {
				bool handled = false;
				for(auto func : inputComp->keyBindings[keyEvent.value().getKey()]) {
					if(func(keyEvent.value()) == InputResponse::handled) {
						handled = true;
						break;
					}
				}
				if(handled) {
					break;
				}
			}
		}

		//Mouse
	}
}