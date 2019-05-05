#pragma once

namespace clv{
	class Window;
	class LayerStack;
	class Layer;
	class Event;
	class WindowCloseEvent;
	class ImGuiLayer;
	namespace scene{ class Scene; };

	class Application{
		//VARIABLES
	private:
		std::unique_ptr<Window> window;
		std::shared_ptr<scene::Scene> scene;

		bool running = true;

		std::unique_ptr<LayerStack> layerStack;
		std::shared_ptr<ImGuiLayer> imGuiLayer;

		static Application* instance;

		std::chrono::system_clock::time_point prevFrameTime;

		//FUNCTIONS
	public:
		CLV_API Application();
		CLV_API virtual ~Application();

		CLV_API void run();
		CLV_API void stop();

		CLV_API void onEvent(Event& e);

		CLV_API void pushLayer(std::shared_ptr<Layer> layer);
		CLV_API void pushOverlay(std::shared_ptr<Layer> overlay);

		CLV_API inline static Application& get();
		CLV_API inline Window& getWindow();
		CLV_API inline std::shared_ptr<scene::Scene> getScene();

	private:
		bool onWindowClose(WindowCloseEvent& e);
	};

	//To be defined in CLIENT
	Application* createApplication();
}

#include "Application.inl"