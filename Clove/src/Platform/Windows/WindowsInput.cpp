#include "clvpch.h"
#include "WindowsInput.h"
#include "Clove/Application.h"

#include <GLFW/glfw3.h>

namespace clv{
	Input* Input::instance = new WindowsInput();

	bool WindowsInput::isKeyPressed_Impl(int keyCode){
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		int state = glfwGetKey(window, keyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::isMouseButtonPressed_Impl(int button){
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::getMousePosition_Impl(){
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		double xPos = 0.0, yPos = 0.0;
		glfwGetCursorPos(window, &xPos, &yPos);
		return { static_cast<float>(xPos), static_cast<float>(yPos) };
	}

	float WindowsInput::getMouseX_Impl(){
		auto[x, y] = getMousePosition_Impl();
		return x;
	}

	float WindowsInput::getMouseY_Impl(){
		auto[x, y] = getMousePosition_Impl();
		return y;
	}
}