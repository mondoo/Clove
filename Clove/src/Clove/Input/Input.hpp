#pragma once

#include "Clove/Input/KeyCodes.hpp"
#include "Clove/Input/MouseButtonCodes.hpp"

namespace clv{
	namespace input{
		CLV_API bool isKeyPressed(Key key);

		CLV_API bool isMouseButtonPressed(MouseButton button);
		CLV_API std::pair<float, float> getMousePosition();
		CLV_API float getMouseX();
		CLV_API float getMouseY();
	};
}