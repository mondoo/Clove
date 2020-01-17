#pragma once

#include "Clove/Platform/Core/PlatformTypes.hpp"
#include "Clove/Graphics/Core/GraphicsTypes.hpp"

#include <chrono>

namespace clv{
	class LayerStack;
	class Layer;
}

namespace tnc::ecs{
	class Manager;
}

namespace clv::plt{
	class Platform;
	class Window;
}

namespace clv::gfx{
	class RenderFactory;
}

namespace tnc{
	enum class ApplicationState{
		running,
		stopped
	};
}

namespace tnc{
	class Application{
		//VARIABLES
	private:
		std::unique_ptr<clv::plt::Platform> platformInstance;
		std::shared_ptr<clv::plt::Window> mainWindow;

		std::unique_ptr<ecs::Manager> ecsManager;

		ApplicationState currentState = ApplicationState::running;

		std::unique_ptr<clv::LayerStack> layerStack;

		static Application* instance;

		std::chrono::system_clock::time_point prevFrameTime;

		//FUNCTIONS
	public:
		Application() = delete;
		Application(clv::gfx::API api);
		Application(clv::gfx::API api, const clv::plt::Window& parentWindow, const clv::mth::vec2i& windowPosition, const clv::mth::vec2i& windowSize);

		virtual ~Application();

		void update();
		void stop();

		ApplicationState getState() const;

		void pushLayer(std::shared_ptr<clv::Layer> layer);
		void pushOverlay(std::shared_ptr<clv::Layer> overlay);

		static Application& get();

		ecs::Manager& getManager();

		clv::plt::Window& getMainWindow() const;
		void setMainWindow(const std::shared_ptr<clv::plt::Window>& window);

		clv::gfx::RenderFactory& getGraphicsFactory();
	};
}