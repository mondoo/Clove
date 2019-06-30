#include "clvpch.hpp"
#include "Application.hpp"

#include "Clove/Platform/Window.hpp"
#include "Clove/Input/Input.hpp"
#include "Clove/LayerStack.hpp"
#include "Clove/Layer.hpp"
#include "Clove/Events/Event.hpp"
#include "Clove/Events/ApplicationEvent.hpp"
#include "Clove/ImGui/ImGuiLayer.hpp"
#include "Clove/Input/Keyboard.hpp"
#include "Clove/Input/Mouse.hpp"
#include "Clove/Events/KeyEvent.hpp"
#include "Clove/Events/MouseEvent.hpp"
#include "Clove/Graphics/Renderer.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/ECS/Systems/Render3DSystem.hpp"

namespace clv{
	Application* Application::instance = nullptr;

	Application::Application(){
		CLV_ASSERT(!instance, "Application already exists!");
		instance = this;

		window = std::unique_ptr<Window>(Window::create());
		window->setEventCallbackFunction(CLV_BIND_FUNCTION_1P(&Application::onEvent, this));
		window->setVSync(true);

		ecsManager.getSystem<ecs::Render3DSystem>()->initialiseRenderer(window->getContext());

		layerStack = std::make_unique<LayerStack>();

	#if CLV_PLATFORM_WINDOWS
		imGuiLayer = std::make_shared<ImGuiLayer>();
		pushLayer(imGuiLayer);
	#else
		CLV_LOG_WARN("IMGUI Disabled for non windows builds");
	#endif

		CLV_LOG_INFO("Successfully initialised Clove");
	}

	Application::~Application() = default;

	void Application::run(){
		while(running){
			auto currFrameTime = std::chrono::system_clock::now();
			std::chrono::duration<float> deltaSeonds = currFrameTime - prevFrameTime;
			prevFrameTime = currFrameTime;

			window->beginFrame();

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

			ecsManager.update(deltaSeonds.count());

		#if CLV_PLATFORM_WINDOWS
			imGuiLayer->begin();
			for(auto layer : *layerStack){
				layer->onImGuiRender();
			}
			imGuiLayer->end();
		#endif
			
			window->endFrame();
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

	Application& Application::get(){
		return *instance;
	}

	Window& Application::getWindow(){
		return *window;
	}

	gfx::Renderer& Application::getRenderer(){
		return ecsManager.getSystem<ecs::Render3DSystem>()->getRenderer();
	}

	ecs::Manager& Application::getManager(){
		return ecsManager;
	}

	bool Application::onWindowClose(WindowCloseEvent& e){
		running = false;
		return true;
	}
}