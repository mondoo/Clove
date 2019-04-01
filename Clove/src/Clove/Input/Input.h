#pragma once

#include "Clove/Input/KeyCodes.h"
#include "Clove/Input/MouseButtonCodes.h"

namespace clv{
	class CLV_API Input{
		//VARIABLES
	private:
		static Input* instance;

		//FUNCTIONS
	public:
		inline static bool isKeyPressed(Key key);

		inline static bool isMouseButtonPressed(MouseButton button);
		inline static std::pair<float, float> getMousePosition();
		inline static float getMouseX();
		inline static float getMouseY();

	private:
		virtual bool isKeyPressed_Impl(Key key) = 0;

		virtual bool isMouseButtonPressed_Impl(MouseButton button) = 0;
		virtual std::pair<float, float> getMousePosition_Impl() = 0;
		virtual float getMouseX_Impl() = 0;
		virtual float getMouseY_Impl() = 0;
	};
}

#include "Clove/Input/Input.inl"