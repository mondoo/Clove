#pragma once

#include "Clove/Core.h"
#include "Clove/Input/KeyCodes.h"
#include "Clove/Input/MouseButtonCodes.h"

namespace clv{
	class CLV_API Input{
		//VARIABLES
	private:
		static Input* instance;

		//FUNCTIONS
	public:
		inline static bool isKeyPressed(Key key){ return instance->isKeyPressed_Impl(key); }

		inline static bool isMouseButtonPressed(MouseButton button){ return instance->isMouseButtonPressed_Impl(button); }
		inline static std::pair<float, float> getMousePosition(){ return instance->getMousePosition_Impl(); }
		inline static float getMouseX(){ return instance->getMouseX_Impl(); }
		inline static float getMouseY(){ return instance->getMouseY_Impl(); }

	private:
		virtual bool isKeyPressed_Impl(Key key) = 0;

		virtual bool isMouseButtonPressed_Impl(MouseButton button) = 0;
		virtual std::pair<float, float> getMousePosition_Impl() = 0;
		virtual float getMouseX_Impl() = 0;
		virtual float getMouseY_Impl() = 0;
	};
}