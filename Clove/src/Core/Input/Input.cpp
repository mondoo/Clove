#include "Clove/Core/Input/Input.hpp"

//#include "Clove/Core/Platform/Application.hpp"
#include "Clove/Core/Platform/Window.hpp"

namespace clv{
	namespace input{
		bool isKeyPressed(Key key){
			//TODO
			//return plt::Platform::get().getMainWindow().getKeyboard().isKeyPressed(key);
			return false;
		}

		bool isMouseButtonPressed(MouseButton button){
			//TODO
			//return plt::Platform::get().getMainWindow().getMouse().isButtonPressed(button);
			return false;
		}

		std::pair<int32, int32> getMousePosition(){
			//TODO
			//return plt::Platform::get().getMainWindow().getMouse().getPosition();
			return { 0, 0 };
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