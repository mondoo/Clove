#pragma once

#include "Clove/Core/Platform/PlatformTypes.hpp"
#include "Clove/Core/Graphics/GraphicsTypes.hpp"

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

		[[nodiscard]] std::shared_ptr<clv::plt::Window> openWindow(clv::plt::WindowType windowType, const clv::plt::WindowProps& props = {});
		[[nodiscard]] std::shared_ptr<clv::plt::Window> openChildWindow(clv::plt::WindowType windowType, const clv::plt::Window& parentWindow, const clv::mth::vec2i& position, const clv::mth::vec2i& size);
	};
}