#pragma once

#include "Bulb/ECS/Component.hpp"

#include <Clove/Input/Keyboard.hpp>
#include <Clove/Input/Mouse.hpp>

namespace blb::ecs {
	enum class InputResponse {
		ignored,
		handled
	};
}

namespace blb::ecs {
	class InputComponent : public Component<InputComponent> {
		friend class InputSystem;

		//VARIABLES
	//private:
	public: //Temp
		std::unordered_map<clv::Key, std::vector<std::function<InputResponse(const clv::Keyboard::Event&)>>> keyBindings;
		std::unordered_map<clv::MouseButton, std::vector<std::function<InputResponse(const clv::Mouse::Event&)>>> mouseButtonBindings;

		//FUNCTIONS
	public:
		InputComponent();

		InputComponent(const InputComponent& other);
		InputComponent(InputComponent&& other) noexcept;

		InputComponent& operator=(const InputComponent& other);
		InputComponent& operator=(InputComponent&& other) noexcept;

		~InputComponent();

		//template<typename FunctionType>

	};
}