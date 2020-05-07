#pragma once

#include "Clove/Input/Keyboard.hpp"
#include "Clove/Input/Mouse.hpp"

namespace clv {
	struct InputEvent{
		Keyboard::Event keyboardEvent;
		Mouse::Event mouseEvent;
	};
}