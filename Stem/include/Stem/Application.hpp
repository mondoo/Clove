#pragma once

#include <Clove/Platform/PlatformTypes.hpp>
#include <Bulb/LayerStack.hpp>

namespace clv::plt {
	class Platform;
	class Window;
}

namespace blb {
	class Layer;
}

namespace stm {
	struct ApplicationDescriptor{
		clv::plt::WindowDescriptor windowDescriptor;
	};
}

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
		~Application();

		void start();
		void run();

		const std::shared_ptr<clv::plt::Window>& getWindow() const;
	};
}

//To be defined by the client
extern stm::ApplicationDescriptor getApplicationDescriptor();
extern std::shared_ptr<blb::Layer> createApplicationLayer(const stm::Application& app);
