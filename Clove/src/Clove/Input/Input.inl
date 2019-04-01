namespace clv{
	inline bool Input::isKeyPressed(Key key){
		return instance->isKeyPressed_Impl(key);
	}

	inline bool Input::isMouseButtonPressed(MouseButton button){
		return instance->isMouseButtonPressed_Impl(button);
	}

	inline std::pair<float, float> Input::getMousePosition(){
		return instance->getMousePosition_Impl();
	}

	inline float Input::getMouseX(){
		return instance->getMouseX_Impl();
	}

	inline float Input::getMouseY(){
		return instance->getMouseY_Impl();
	}
}