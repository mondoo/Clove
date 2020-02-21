#pragma once

#include "Clove/Core/Input/KeyCodes.hpp"
#include "Clove/Core/Input/MouseButtonCodes.hpp"

namespace tnc::ipt{
	bool isKeyPressed(clv::Key key);

	bool isMouseButtonPressed(clv::MouseButton button);
	clv::mth::vec2i getMousePosition();
	int32_t getMouseX();
	int32_t getMouseY();
}