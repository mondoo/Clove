#pragma once

#include "Clove/Core/Input/KeyCodes.hpp"
#include "Clove/Core/Input/MouseButtonCodes.hpp"

namespace tnc::ipt{
	bool isKeyPressed(clv::Key key);

	bool isMouseButtonPressed(clv::MouseButton button);
	std::pair<clv::int32, clv::int32> getMousePosition();
	clv::int32 getMouseX();
	clv::int32 getMouseY();
}