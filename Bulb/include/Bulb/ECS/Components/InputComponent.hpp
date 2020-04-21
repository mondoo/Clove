#pragma once

#include "Bulb/ECS/Component.hpp"

#include <Clove/Input/KeyCodes.hpp>
#include <Clove/Input/MouseButtonCodes.hpp>

namespace blb::ecs {
	enum class InputResponse {

	};
}

namespace blb::ecs {
	class InputComponent : public Component<InputComponent> {
		friend class InputSystem;

		//VARIABLES
	//private:
	public: //Temp
		std::unordered_map<clv::Key, std::vector<std::function<InputResponse()>>> keyBindings;
		std::unordered_map<clv::MouseButton, std::vector<std::function<InputResponse()>>> mouseButtonBindings;

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