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
			const Keyboard::Event& event		= keyEvent.value();
			
			const Key key						= event.getKey();
			const Keyboard::Event::Type	type	= event.getType();

			auto keyState = std::make_pair(key, type);
			
			for(auto&& [inputComp] : inputSets) {
				bool consumed = false;
				for(auto func : inputComp->keyBindings[keyState]) {
					if(func() == InputResponse::Consumed) {
						consumed = true;
						break;
					}
				}
				if(consumed) {
					break;
				}
			}
		}

		//Mouse
		while(std::optional<Mouse::Event> mouseEvent = window->getMouse().getEvent()) {
			const Mouse::Event& event		= mouseEvent.value();
			
			const MouseButton button		= event.getButton();
			const Mouse::Event::Type type	= event.getType();

			auto buttonState = std::make_pair(button, type);

			for(auto&& [inputComp] : inputSets) {
				bool consumed = false;
				for(auto func : inputComp->mouseButtonBindings[buttonState]) {
					if(func() == InputResponse::Consumed) {
						consumed = true;
						break;
					}
				}
				if(consumed) {
					break;
				}
			}
		}
	}
}