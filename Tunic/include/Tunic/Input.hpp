#pragma once

#include "Clove/Core/Input/KeyCodes.hpp"
#include "Clove/Core/Input/MouseButtonCodes.hpp"

namespace tnc::ipt{
	bool isKeyPressed(clv::Key key);

	bool isMouseButtonPressed(clv::MouseButton button);
	std::pair<int32_t, int32_t> getMousePosition();
	int32_t getMouseX();
	int32_t getMouseY();
}