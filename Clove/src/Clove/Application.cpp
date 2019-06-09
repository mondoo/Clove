#include "clvpch.hpp"
#include "Application.hpp"

#include "Clove/Platform/Window.hpp"
#include "Clove/Input/Input.hpp"
#include "Clove/LayerStack.hpp"
#include "Clove/Layer.hpp"
#include "Clove/Events/Event.hpp"
#include "Clove/Events/ApplicationEvent.hpp"
#include "Clove/ImGui/ImGuiLayer.hpp"
#include "Clove/Scene/Scene.hpp"
#include "Clove/Input/Keyboard.hpp"
#include "Clove/Input/Mouse.hpp"
#include "Clove/Events/KeyEvent.hpp"
#include "Clove/Events/MouseEvent.hpp"

#include "Clove/Graphics/Renderer.hpp"

namespace clv{
	Application* Application::instance = nullptr;

	Application::Application(){
		CLV_ASSERT(!instance, "Application already exists!");
		instance = this;

		window = std::unique_ptr<Window>(Window::create({ "Clove Engine", 1920, 1080 }));
		window->setEventCallbackFunction(CLV_BIND_FUNCTION_1P(&Application::onEvent, this));

		//
		ECSManager.getRenderer() = gfx::Renderer::createRenderer(*window, gfx::API::DirectX11);
		//

		scene = std::make_shared<scene::Scene>();

		layerStack = std::make_unique<LayerStack>();

		imGuiLayer = std::make_shared<ImGuiLayer>();
		pushLayer(imGuiLayer);
	}

	Application::~Application() = default;

	void Application::run(){
		while(running){
			auto currFrameTime = std::chrono::system_clock::now();
			std::chrono::duration<float> deltaSeonds = currFrameTime - prevFrameTime;
			prevFrameTime = currFrameTime;

			window->beginFrame();

			//
			ECSManager.getRenderer()->clear();
			//

			//Temp input handling
			while(auto e = getWindow().getKeyboard().getKeyEvent()){
				Keyboard::Event keyEvent = e.value();
				if(keyEvent.isPressed()){
					KeyPressedEvent kpe(keyEvent.getKey(), -1); //TODO: might need to get rid of the keyevent
					onEvent(kpe);
				} else if(keyEvent.isReleased()){
					KeyReleasedEvent kre(keyEvent.getKey());
					onEvent(kre);
				}
			}

			while(auto e = getWindow().getKeyboard().getCharEvent()){
				char character = e.value();
				KeyTypedEvent kte(static_cast<Key>(character));
				onEvent(kte);
			}

			while(auto e = getWindow().getMouse().getEvent()){
				Mouse::Event mouseEvent = e.value();
				switch(mouseEvent.getType()){
					case Mouse::Event::Type::Move:
						{
							const auto[x, y] = mouseEvent.getPos();
							MouseMovedEvent mme(x, y);
							onEvent(mme);
						}
						break;
					case Mouse::Event::Type::Pressed:
						{
							MouseButtonPressedEvent mbpe(mouseEvent.getButton());
							onEvent(mbpe);
						}
						break;
					case Mouse::Event::Type::Released:
						{
							MouseButtonReleasedEvent mbre(mouseEvent.getButton());
							onEvent(mbre);
						}
						break;
					case Mouse::Event::Type::WheelUp:
						{
							MouseScrolledEvent mse(1, 0);
							onEvent(mse);
						}
						break;
					case Mouse::Event::Type::WheelDown:
						{
							MouseScrolledEvent mse(-1, 0);
							onEvent(mse);
						}
						break;
				}
			}
			//~Temp


			for(auto layer : *layerStack){
				layer->onUpdate();
			}

			scene->update(deltaSeonds.count());

			imGuiLayer->begin();
			for(auto layer : *layerStack){
				layer->onImGuiRender();
			}
			imGuiLayer->end();
			
			window->endFrame();
			ECSManager.update(deltaSeonds.count());
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

	gfx::Renderer& Application::getRenderer(){
		return *ECSManager.getRenderer();
	}

	bool Application::onWindowClose(WindowCloseEvent& e){
		running = false;
		return true;
	}
}