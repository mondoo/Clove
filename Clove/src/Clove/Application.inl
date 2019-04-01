namespace clv{
	inline Application& Application::get(){
		return *instance;
	}

	inline Window& Application::getWindow(){
		return *window;
	}
}