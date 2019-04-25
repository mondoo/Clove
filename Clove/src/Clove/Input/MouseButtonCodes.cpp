#include "clvpch.hpp"
#include "MouseButtonCodes.hpp"

#include "Clove/Log.hpp"

#include <GLFW/glfw3.h>

namespace clv{
	namespace MouseButtonCodeHelpers{
		int CloveToGLFW(MouseButton button){
			switch(button){
				case MouseButton::_1:
					return GLFW_MOUSE_BUTTON_1;
				case MouseButton::_2:
					return GLFW_MOUSE_BUTTON_2;
				case MouseButton::_3:
					return GLFW_MOUSE_BUTTON_3;
				case MouseButton::_4:
					return GLFW_MOUSE_BUTTON_4;
				case MouseButton::_5:
					return GLFW_MOUSE_BUTTON_5;
				case MouseButton::_6:
					return GLFW_MOUSE_BUTTON_6;
				case MouseButton::_7:
					return GLFW_MOUSE_BUTTON_7;
				case MouseButton::_8:
					return GLFW_MOUSE_BUTTON_8;
				default:
					CLV_WARN("Invalid mouse button passed in, returning -1");
					return -1;
			}
		}

		MouseButton GLFWToClove(int button){
			switch(button){
				case GLFW_MOUSE_BUTTON_1:
					return MouseButton::_1;
				case GLFW_MOUSE_BUTTON_2:
					return MouseButton::_2;
				case GLFW_MOUSE_BUTTON_3:
					return MouseButton::_3;
				case GLFW_MOUSE_BUTTON_4:
					return MouseButton::_4;
				case GLFW_MOUSE_BUTTON_5:
					return MouseButton::_5;
				case GLFW_MOUSE_BUTTON_6:
					return MouseButton::_6;
				case GLFW_MOUSE_BUTTON_7:
					return MouseButton::_7;
				case GLFW_MOUSE_BUTTON_8:
					return MouseButton::_8;
				default:
					return MouseButton::_None;
			}
		}
	}
}