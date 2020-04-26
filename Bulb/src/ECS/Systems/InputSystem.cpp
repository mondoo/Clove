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
			const Keyboard::Event& event	= keyEvent.value();
			const Key key					= event.getKey();
			
			for(auto&& [inputComp] : inputSets) {
				bool consumed = false;
				for(auto func : inputComp->keyBindings[key]) {
					if(func(event) == InputResponse::Consumed) {
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
			const Mouse::Event& event	= mouseEvent.value();
			const MouseButton button	= event.getButton();

			for(auto&& [inputComp] : inputSets) {
				bool consumed = false;
				for(auto func : inputComp->mouseButtonBindings[button]) {
					if(func(event) == InputResponse::Consumed) {
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