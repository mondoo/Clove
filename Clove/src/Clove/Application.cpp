#include "clvpch.hpp"
#include "Application.hpp"

#include "Clove/Window.hpp"
#include "Clove/Input/Input.hpp"
#include "Clove/LayerStack.hpp"
#include "Clove/Layer.hpp"
#include "Clove/Events/Event.hpp"
#include "Clove/Events/ApplicationEvent.hpp"
#include "Clove/ImGui/ImGuiLayer.hpp"
#include "Clove/Rendering/Renderer.hpp"
#include "Clove/Scene/Scene.hpp"

namespace clv{
	Application* Application::instance = nullptr;

	Application::Application(){
		CLV_ASSERT(!instance, "Application already exists!");
		instance = this;

		window = std::unique_ptr<Window>(Window::create({ "Clove Engine", 1920, 1080 }));
		window->setEventCallbackFunction(CLV_BIND_FUNCTION_1P(&Application::onEvent, this));

		layerStack = std::make_unique<LayerStack>();

		imGuiLayer = std::make_shared<ImGuiLayer>();
		pushLayer(imGuiLayer);

		renderer = std::make_unique<Renderer>();
		scene = std::make_shared<scene::Scene>();
	}

	Application::~Application() = default;

	void Application::run(){
		while(running){
			auto currFrameTime = std::chrono::system_clock::now();
			std::chrono::duration<float> deltaSeonds = currFrameTime - prevFrameTime;
			prevFrameTime = currFrameTime;

			window->swapBuffers();
			renderer->clear();
			
			for(auto layer : *layerStack){
				layer->onUpdate();
			}

			scene->update(deltaSeonds.count());
			renderer->drawQueue(scene);
			
			imGuiLayer->begin();
			for(auto layer : *layerStack){
				layer->onImGuiRender();
			}
			imGuiLayer->end();
		}
	}

	void Application::stop(){
		running = false;
	}

	void Application::onEvent(Event& e){
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(CLV_BIND_FUNCTION_1P(&Application::onWindowClose, this));

		if(e.isHandled()){
			return;
		}

		for(auto it = layerStack->end(); it != layerStack->begin(); ){
			(*--it)->onEvent(e);
			if(e.isHandled()){
				break;
			}
		}
	}

	void Application::pushLayer(std::shared_ptr<Layer> layer){
		layerStack->pushLayer(layer);
	}

	void Application::pushOverlay(std::shared_ptr<Layer> overlay){
		layerStack->pushOverlay(overlay);
	}

	bool Application::onWindowClose(WindowCloseEvent& e){
		running = false;
		return true;
	}
}