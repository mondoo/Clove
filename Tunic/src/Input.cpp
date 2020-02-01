#include "Tunic/Input.hpp"

#include "Tunic/Application.hpp"
#include "Clove/Platform/Core/Window.hpp"

using namespace clv;

namespace tnc::ipt{
	bool isKeyPressed(Key key){
		return tnc::Application::get().getMainWindow().getKeyboard().isKeyPressed(key);
	}

	bool isMouseButtonPressed(MouseButton button){
		return tnc::Application::get().getMainWindow().getMouse().isButtonPressed(button);
	}

	std::pair<int32_t, int32_t> getMousePosition(){
		return tnc::Application::get().getMainWindow().getMouse().getPosition();
	}

	int32_t getMouseX(){
		auto [x, y] = getMousePosition();
		return x;
	}

	int32_t getMouseY(){
		auto [x, y] = getMousePosition();
		return y;
	}
}