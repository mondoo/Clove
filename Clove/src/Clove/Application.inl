namespace clv{
	inline Application& Application::get(){
		return *instance;
	}

	inline Window& Application::getWindow(){
		return *window;
	}

	inline std::shared_ptr<scene::Scene> Application::getScene(){
		return scene;
	}
}