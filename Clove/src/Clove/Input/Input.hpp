#pragma once

#include "Clove/Input/KeyCodes.hpp"
#include "Clove/Input/MouseButtonCodes.hpp"

namespace clv{
	class Input{
		//VARIABLES
	private:
		static Input* instance;

		//FUNCTIONS
	public:
		CLV_API inline static bool isKeyPressed(Key key);

		CLV_API inline static bool isMouseButtonPressed(MouseButton button);
		CLV_API inline static std::pair<float, float> getMousePosition();
		CLV_API inline static float getMouseX();
		CLV_API inline static float getMouseY();

	private:
		virtual bool isKeyPressed_Impl(Key key) = 0;

		virtual bool isMouseButtonPressed_Impl(MouseButton button) = 0;
		virtual std::pair<float, float> getMousePosition_Impl() = 0;
		virtual float getMouseX_Impl() = 0;
		virtual float getMouseY_Impl() = 0;
	};
}

#include "Input.inl"