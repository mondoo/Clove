#include "clvpch.h"
#include "Application.h"

#include "Clove/Log.h"

//TODO: Remove
#include <glad/glad.h>

namespace clv{
	Application* Application::instance = nullptr;

	Application::Application(){
		CLV_CORE_ASSERT(!instance, "Application already exists!");
		instance = this;

		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallbackFunction(BIND_FUNCTION_OneParam(&Application::onEvent, this));
	}

	void Application::run(){
		while(running){
			//TODO: Remove
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			//

			for(Layer* layer : layerStack){
				layer->onUpdate();
			}

			window->onUpdate();
		}
	}
	void Application::onEvent(Event& e){
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_FUNCTION_OneParam(&Application::onWindowClose, this));

		for(auto it = layerStack.end(); it != layerStack.begin(); ){
			(*--it)->onEvent(e);
			if(e.isHandled()){
				break;
			}
		}
	}

	void Application::pushLayer(Layer* layer){
		layerStack.pushLayer(layer);
	}

	void Application::pushOverlay(Layer* overlay){
		layerStack.pushOverlay(overlay);
	}

	bool Application::onWindowClose(WindowCloseEvent& e){
		running = false;
		return true;
	}
}