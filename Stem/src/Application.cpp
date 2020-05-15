#include "Stem/Application.hpp"

#include <Bulb/Bulb.hpp>

namespace stm {
	Application::Application() = default;

	Application::~Application() = default;

	void Application::start(){
		platformInstance = clv::plt::createPlatformInstance();
		window = platformInstance->createWindow(getApplicationDescriptor().windowDescriptor);
		window->setVSync(true);

		layerStack.pushLayer(createApplicationLayer(*this));

		prevFrameTime = std::chrono::system_clock::now();
	}

	void Application::run() {
		while(window->isOpen()){
			auto currFrameTime = std::chrono::system_clock::now();
			std::chrono::duration<float> deltaSeonds = currFrameTime - prevFrameTime;
			prevFrameTime = currFrameTime;

			window->beginFrame();

			//Respond to input
			while(auto keyEvent = window->getKeyboard().getKeyEvent()) {
				const clv::InputEvent event{ *keyEvent, clv::InputEventType::Keyboard };
				for(const auto& layer : layerStack) {
					if(layer->onInputEvent(event) == blb::InputResponse::Consumed) {
						break;
					}
				}
			}
			while(auto mouseEvent = window->getMouse().getEvent()) {
				const clv::InputEvent event{ *mouseEvent, clv::InputEventType::Mouse };
				for(const auto& layer : layerStack) {
					if(layer->onInputEvent(event) == blb::InputResponse::Consumed) {
						break;
					}
				}
			}

			//Do frame logic
			for(const auto& layer : layerStack) {
				layer->onUpdate(deltaSeonds.count());
			}

			window->endFrame();
		}
	}
	
	const std::shared_ptr<clv::plt::Window>& Application::getWindow() const {
		return window;
	}
}