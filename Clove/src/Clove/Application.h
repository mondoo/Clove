#pragma once

namespace clv{
	class Window;
	class LayerStack;
	class Layer;
	class Event;
	class WindowCloseEvent;
	class ImGuiLayer;

	class CLV_API Application{
		//VARIABLES
	private:
		std::unique_ptr<Window> window;
		std::shared_ptr<ImGuiLayer> imGuiLayer;

		bool running = true;
		std::unique_ptr<LayerStack> layerStack;

		static Application* instance;
		
		//FUNCTIONS
	public:
		Application();
		virtual ~Application();

		void run();

		void onEvent(Event& e);

		void pushLayer(std::shared_ptr<Layer> layer);
		void pushOverlay(std::shared_ptr<Layer> overlay);

		inline static Application& get();
		inline Window& getWindow();

	private:
		bool onWindowClose(WindowCloseEvent& e);
	};

	//To be defined in CLIENT
	Application* createApplication();
}

#include "Clove/Application.inl"