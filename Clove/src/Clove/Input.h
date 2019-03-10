#pragma once

#include "Clove/Core.h"

namespace clv{
	class CLV_API Input{
		//VARIABLES
	private:
		static Input* instance;

		//FUNCTIONS
	public:
		inline static bool isKeyPressed(int keyCode){ return instance->isKeyPressed_Impl(keyCode); }

		inline static bool isMouseButtonPressed(int button){ return instance->isMouseButtonPressed_Impl(button); }
		inline static std::pair<float, float> getMousePosition(){ return instance->getMousePosition_Impl(); }
		inline static float getMouseX(){ return instance->getMouseX(); }
		inline static float getMouseY(){ return instance->getMouseY(); }

	private:
		virtual bool isKeyPressed_Impl(int keyCode) = 0;

		virtual bool isMouseButtonPressed_Impl(int button) = 0;
		virtual std::pair<float, float> getMousePosition_Impl() = 0;
		virtual float getMouseX_Impl() = 0;
		virtual float getMouseY_Impl() = 0;
	};
}