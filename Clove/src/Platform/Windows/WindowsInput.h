#pragma once

#include "Clove/Input.h"

namespace clv{
	class WindowsInput : public Input{
		//FUNCTIONS
	private:
		virtual bool isKeyPressed_Impl(Key key) override;

		virtual bool isMouseButtonPressed_Impl(int button) override;
		virtual std::pair<float, float> getMousePosition_Impl() override;
		virtual float getMouseX_Impl() override;
		virtual float getMouseY_Impl() override;
	};
}