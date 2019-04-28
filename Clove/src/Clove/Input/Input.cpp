#include "clvpch.hpp"
#include "Clove/Input/Input.hpp"

#include "Clove/Application.hpp"
#include "Clove/Window.hpp"

namespace clv{
	namespace input{
		bool isKeyPressed(Key key){
			return Application::get().getWindow().getKeyboard().isKeyPressed(key);
		}

		bool isMouseButtonPressed(MouseButton button){
			return Application::get().getWindow().getMouse().isButtonPressed(button);
		}

		std::pair<float, float> getMousePosition(){
			return Application::get().getWindow().getMouse().getPosition();
		}

		float getMouseX(){
			auto[x, y] = getMousePosition();
			return x;
		}

		float getMouseY(){
			auto[x, y] = getMousePosition();
			return y;
		}
	}
}