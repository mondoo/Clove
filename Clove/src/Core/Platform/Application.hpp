#pragma once

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
		std::unique_ptr<plt::Window> window;
		std::unique_ptr<ecs::Manager> ecsManager;

		ApplicationState currentState = ApplicationState::running;

		std::unique_ptr<LayerStack> layerStack;

		static Application* instance;

		std::chrono::system_clock::time_point prevFrameTime;

		//FUNCTIONS
	public:
		Application();
		virtual ~Application();

		void update();
		ApplicationState getState() const;

		void stop();

		void pushLayer(std::shared_ptr<Layer> layer);
		void pushOverlay(std::shared_ptr<Layer> overlay);

		static Application& get();
		
		plt::Window& getWindow();
		ecs::Manager& getManager();

		static std::unique_ptr<Application> createApplication();

	private:
		void onWindowClose();

		virtual std::unique_ptr<Window> createWindow(const WindowProps& props = WindowProps()) = 0;
		virtual std::unique_ptr<Window> createWindow(const WindowProps& props, gfx::API api) = 0;
	};
}