#include "clvpch.h"
#include "Application.h"
#include "Clove/Input/Input.h"

namespace clv{
	Application* Application::instance = nullptr;

	Application::Application(){
		CLV_ASSERT(!instance, "Application already exists!");
		instance = this;

		window = std::unique_ptr<Window>(Window::create({ "Clove Engine", 1920, 1080 }));
		window->setEventCallbackFunction(CLV_BIND_FUNCTION_1P(&Application::onEvent, this));

		imGuiLayer = std::make_shared<ImGuiLayer>(ImGuiLayer());
		pushLayer(imGuiLayer);
	}

	void Application::run(){
		while(running){
			for(auto layer : layerStack){
				layer->onUpdate();
			}

			imGuiLayer->begin();
			for(auto layer : layerStack){
				layer->onImGuiRender();
			}
			imGuiLayer->end();

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

	void Application::pushLayer(std::shared_ptr<Layer> layer){
		layerStack.pushLayer(layer);
	}

	void Application::pushOverlay(std::shared_ptr<Layer> overlay){
		layerStack.pushOverlay(overlay);
	}

	bool Application::onWindowClose(WindowCloseEvent& e){
		running = false;
		return true;
	}
}