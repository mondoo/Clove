#include "Stem/Application.hpp"

#include <Bulb/Bulb.hpp>

namespace stm {
	Application::Application() {
		clv::Log::init();

		platformInstance = clv::plt::createPlatformInstance();
		window = platformInstance->createWindow({ "NO_NAME", 1280, 720 });
		window->setVSync(true);

		layerStack.pushLayer(createApplicationLayer());

		prevFrameTime = std::chrono::system_clock::now();
	}

	void Application::run() {
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