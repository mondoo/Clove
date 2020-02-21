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

	mth::vec2i getMousePosition(){
		auto [x, y] = tnc::Application::get().getMainWindow().getMouse().getPosition();
		return { x, y };
	}

	int32_t getMouseX(){
		return getMousePosition().x;
	}

	int32_t getMouseY(){
		return getMousePosition().y;
	}
}