#pragma once

#include <Bulb/LayerStack.hpp>

namespace clv::plt {
	class Platform;
	class Window;
}

namespace blb {
	class Layer;
}

namespace stm {
	class Application {
		//VARIABLES
	//Temp
	//private:
	public:
		std::unique_ptr<clv::plt::Platform> platformInstance;
		std::shared_ptr<clv::plt::Window> window;

		blb::LayerStack layerStack;

		std::chrono::system_clock::time_point prevFrameTime;

		//FUNCTIONS
	public:
		Application();

		void start();
		void run();
	};
}

extern std::shared_ptr<blb::Layer> createApplicationLayer(const stm::Application& app);
