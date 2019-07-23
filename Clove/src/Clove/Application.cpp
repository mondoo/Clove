#include "clvpch.hpp"
#include "Application.hpp"

#include "Clove/Platform/Window.hpp"
#include "Clove/Input/Input.hpp"
#include "Clove/LayerStack.hpp"
#include "Clove/Layer.hpp"
#include "Clove/Events/Event.hpp"
#include "Clove/Events/ApplicationEvent.hpp"
#include "Clove/Input/Keyboard.hpp"
#include "Clove/Input/Mouse.hpp"
#include "Clove/Events/KeyEvent.hpp"
#include "Clove/Events/MouseEvent.hpp"
#include "Clove/Utils/Time.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/RenderCommand.hpp"

#include "Clove/Events/Event.hpp"

namespace clv{
	Application* Application::instance = nullptr;

	Application::Application(){
		CLV_ASSERT(!instance, "Application already exists!");
		instance = this;

		evt::InternalEventDispatcher<WindowCloseEvent>::bind(&Application::onWindowClose, this);

		window = std::unique_ptr<Window>(Window::create());
		window->setVSync(true);

		gfx::RenderCommand::initialiseRenderAPI(window->getContext());
		gfx::RenderCommand::setClearColour({ 1.0f, 0.54f, 0.1f, 1.0f });

		gfx::Renderer::initialise();

		ecsManager = std::make_unique<ecs::Manager>();
		layerStack = std::make_unique<LayerStack>();

		CLV_LOG_INFO("Successfully initialised Clove");

		prevFrameTime = std::chrono::system_clock::now();
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
				layer->onUpdate(deltaSeonds.count());
			}

			gfx::RenderCommand::clear();

			gfx::Renderer::beginScene();

			ecsManager->update(deltaSeonds.count());

            gfx::Renderer::endScene();
			
			window->endFrame();
		}
	}

	void Application::stop(){
		running = false;
	}

	void Application::onEvent(Event& e){
		//-Make events not blocking (some sort of queue)
		//-Integrate new delegate system
		//-Consder ways to make the event system better for people using the engine

		//What would be nice is if classes could bind to the event dispatcher and then you can tell it to
		//dispatch an event, it will add it to the dispatch queue and then that can be broadcast to all
		//listeners

		/*EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(CLV_BIND_FUNCTION_1P(&Application::onWindowClose, this));*/

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

	ecs::Manager& Application::getManager(){
		return *ecsManager;
	}

	evt::HandledType Application::onWindowClose(WindowCloseEvent& e){
		running = false;
		return evt::HandledType::handled_stop;
	}
}