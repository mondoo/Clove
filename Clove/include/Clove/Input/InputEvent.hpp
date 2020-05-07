#pragma once

#include "Clove/Input/Keyboard.hpp"
#include "Clove/Input/Mouse.hpp"

#include <optional>

namespace clv {
	struct InputEvent{
		std::optional<Keyboard::Event> keyboardEvent;
		std::optional<Mouse::Event> mouseEvent;
	};
}