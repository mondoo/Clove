#pragma once

namespace clv::ecs{
	class Manager;
}

namespace clv{
	class Window;
	class LayerStack;
	class Layer;
	class ImGuiLayer;

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

		void update();
		ApplicationState getState() const;

		void stop();

		void pushLayer(std::shared_ptr<Layer> layer);
		void pushOverlay(std::shared_ptr<Layer> overlay);

		static Application& get();
		
		Window& getWindow();
		ecs::Manager& getManager();

	private:
		void onWindowClose();
	};

	//To be defined in CLIENT
	//Application* createApplication();
}