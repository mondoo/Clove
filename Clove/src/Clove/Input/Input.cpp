#include "clvpch.hpp"
#include "Clove/Input/Input.hpp"

#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"

namespace clv{
	namespace input{
		bool isKeyPressed(Key key){
			return Application::get().getWindow().getKeyboard().isKeyPressed(key);
		}

		bool isMouseButtonPressed(MouseButton button){
			return Application::get().getWindow().getMouse().isButtonPressed(button);
		}

		std::pair<int32, int32> getMousePosition(){
			return Application::get().getWindow().getMouse().getPosition();
		}

		int32 getMouseX(){
			auto[x, y] = getMousePosition();
			return x;
		}

		int32 getMouseY(){
			auto[x, y] = getMousePosition();
			return y;
		}
	}
}