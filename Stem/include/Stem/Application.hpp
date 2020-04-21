#pragma once

#include <Bulb/LayerStack.hpp>

namespace clv::plt {
	class Platform;
	class Window;
}

namespace blb {
	class Layer;
}

extern std::shared_ptr<blb::Layer> createApplicationLayer();

namespace stm {
	class Application {
		//VARIABLES
	private:
		std::unique_ptr<clv::plt::Platform> platformInstance;
		std::shared_ptr<clv::plt::Window> window;

		blb::LayerStack layerStack;

		std::chrono::system_clock::time_point prevFrameTime;

		//FUNCTIONS
	public:
		Application();

		void run();
	};
}