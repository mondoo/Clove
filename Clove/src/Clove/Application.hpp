#pragma once

namespace clv::ecs{
	class Manager;
}

namespace clv::gfx{
	class Renderer;
}

namespace clv::evt{
	enum class HandledType;
}

namespace clv{
	class Window;
	class LayerStack;
	class Layer;
	class ImGuiLayer;

	class Application{
		//VARIABLES
	private:
		std::unique_ptr<Window> window;
		std::unique_ptr<ecs::Manager> ecsManager;

		bool running = true;

		std::unique_ptr<LayerStack> layerStack;

		static Application* instance;

		std::chrono::system_clock::time_point prevFrameTime;

		//FUNCTIONS
	public:
		Application();
		virtual ~Application();

		Application(std::unique_ptr<Window>&& window);

		void run();
		void stop();

		void pushLayer(std::shared_ptr<Layer> layer);
		void pushOverlay(std::shared_ptr<Layer> overlay);

		static Application& get();
		
		Window& getWindow();
		ecs::Manager& getManager();

	private:
		void onWindowClose();

		void initialise(std::unique_ptr<Window>&& window);
	};

	//To be defined in CLIENT
	Application* createApplication();
}