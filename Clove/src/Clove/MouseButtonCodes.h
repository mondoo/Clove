#pragma once

namespace clv{
	enum class MouseButton{
		_None = 0,

		_1,
		_2,
		_3,
		_4,
		_5,
		_6,
		_7,
		_8,
		Last	= _8,
		Left	= _1,
		Right	= _2,
		Middle	= _3,
	};

	namespace MouseButtonCodeHelpers{
		int CloveToGLFW(MouseButton button);
		MouseButton GLFWToClove(int button);
	}
}

#define GLFW_MOUSE_BUTTON_1         0
#define GLFW_MOUSE_BUTTON_2         1
#define GLFW_MOUSE_BUTTON_3         2
#define GLFW_MOUSE_BUTTON_4         3
#define GLFW_MOUSE_BUTTON_5         4
#define GLFW_MOUSE_BUTTON_6         5
#define GLFW_MOUSE_BUTTON_7         6
#define GLFW_MOUSE_BUTTON_8         7
#define GLFW_MOUSE_BUTTON_LAST      GLFW_MOUSE_BUTTON_8
#define GLFW_MOUSE_BUTTON_LEFT      GLFW_MOUSE_BUTTON_1
#define GLFW_MOUSE_BUTTON_RIGHT     GLFW_MOUSE_BUTTON_2
#define GLFW_MOUSE_BUTTON_MIDDLE    GLFW_MOUSE_BUTTON_3