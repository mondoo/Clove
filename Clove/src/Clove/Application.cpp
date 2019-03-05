#include "clvpch.h"
#include "Application.h"

#include "Clove/Log.h"

//TODO: Remove
#include <GLFW/glfw3.h>

namespace clv{
	Application::Application(){
		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallbackFunction(std::bind(&Application::onEvent, this, std::placeholders::_1));
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
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowClose, this, std::placeholders::_1));

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