#pragma once

#include "Clove/Input/Keyboard.hpp"
#include "Clove/Input/Mouse.hpp"

#include <variant>

namespace clv {
	enum class InputEventType{
		Keyboard,
		Mouse
	};

	struct InputEvent{
		std::variant<Keyboard::Event, Mouse::Event> event;
		InputEventType eventType;
	};
}