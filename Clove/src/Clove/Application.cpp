#include "clvpch.h"
#include "Application.h"

#include "Clove/Log.h"

#include "Clove/Input.h"

namespace clv{
	Application* Application::instance = nullptr;

	Application::Application(){
		CLV_CORE_ASSERT(!instance, "Application already exists!");
		instance = this;

		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallbackFunction(CLV_BIND_FUNCTION_1P(&Application::onEvent, this));

		//TEMP: DISABLE IMGUI
		/*imGuiLayer = new ImGuiLayer();
		pushLayer(imGuiLayer);*/
	}

	void Application::run(){
		while(running){
			for(Layer* layer : layerStack){
				layer->onUpdate();
			}

			//TEMP: DISABLE IMGUI
			/*imGuiLayer->begin();
			for(Layer* layer : layerStack){
				layer->onImGuiRender();
			}
			imGuiLayer->end();*/

			window->onUpdate();
		}
	}
	void Application::onEvent(Event& e){
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(CLV_BIND_FUNCTION_1P(&Application::onWindowClose, this));

		if(e.isHandled()){
			return;
		}

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