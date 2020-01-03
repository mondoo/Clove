#pragma once

#include "Clove/Core/Platform/PlatformTypes.hpp"
#include "Clove/Core/Graphics/GraphicsTypes.hpp"

#include <chrono>

namespace clv{
	class LayerStack;
	class Layer;
}

namespace clv::ecs{
	class Manager;
}

namespace clv::plt{
	class Window;

	enum class ApplicationState{
		running,
		stopped
	};
}

namespace clv::plt{
	class Application{
		//VARIABLES
	private:
		std::shared_ptr<Window> mainWindow;
		std::unique_ptr<ecs::Manager> ecsManager;

		ApplicationState currentState = ApplicationState::running;

		std::unique_ptr<LayerStack> layerStack;

		static Application* instance;

		std::chrono::system_clock::time_point prevFrameTime;

		//FUNCTIONS
	public:
		Application() = delete;
		Application(gfx::API api);

		virtual ~Application();

		void update();
		void stop();

		ApplicationState getState() const;

		void pushLayer(std::shared_ptr<Layer> layer);
		void pushOverlay(std::shared_ptr<Layer> overlay);

		static Application& get();
		
		ecs::Manager& getManager();

		Window& getMainWindow() const;
		void setMainWindow(const std::shared_ptr<Window>& window);

		[[nodiscard]] std::shared_ptr<Window> openWindow(WindowType windowType, const WindowProps& props = {});
		[[nodiscard]] std::shared_ptr<Window> openChildWindow(WindowType windowType, const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size);

		static std::unique_ptr<Application> createApplication(gfx::API api);
		static gfx::API getPreferedAPI();

	private:
		virtual std::shared_ptr<Window> createWindow(const WindowProps& props) = 0;
		virtual std::shared_ptr<Window> createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size) = 0;
	};
}