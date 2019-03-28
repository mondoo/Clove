#pragma once

#include "Clove/Input/Input.h"

namespace clv{
	class WindowsInput : public Input{
		//FUNCTIONS
	private:
		virtual bool isKeyPressed_Impl(Key key) override;

		virtual bool isMouseButtonPressed_Impl(MouseButton button) override;
		virtual std::pair<float, float> getMousePosition_Impl() override;
		virtual float getMouseX_Impl() override;
		virtual float getMouseY_Impl() override;
	};
}