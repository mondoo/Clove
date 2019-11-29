#pragma once

#include "Clove/Core/Platform/PlatformTypes.hpp"
#include "Clove/Core/Graphics/GraphicsTypes.hpp"

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

	class Application{
		//VARIABLES
	private:
		std::unique_ptr<Window> window;
		std::unique_ptr<ecs::Manager> ecsManager;

		ApplicationState currentState = ApplicationState::running;

		std::unique_ptr<LayerStack> layerStack;

		static Application* instance;

		std::chrono::system_clock::time_point prevFrameTime;

		//FUNCTIONS
	public:
		Application();
		virtual ~Application();

		void start();
		void update();
		void stop();

		ApplicationState getState() const;

		void pushLayer(std::shared_ptr<Layer> layer);
		void pushOverlay(std::shared_ptr<Layer> overlay);

		static Application& get();
		
		Window& getWindow();
		ecs::Manager& getManager();

		static std::unique_ptr<Application> createApplication();

	private:
		void onWindowClose();

		virtual gfx::API getPlatformPreferedAPI() = 0;
		virtual std::unique_ptr<Window> createWindow(const WindowProps& props = WindowProps()) = 0;
	};
}