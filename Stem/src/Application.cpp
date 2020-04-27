#include "Stem/Application.hpp"

#include <Bulb/Bulb.hpp>

namespace stm {
	Application::Application() = default;

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

			for(const auto& layer : layerStack) {
				layer->onUpdate(deltaSeonds.count());
			}

			window->endFrame();
		}
	}
}