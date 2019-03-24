#pragma once

#include "Core.h"

#include "Window.h"
#include "Clove/LayerStack.h"
#include "Clove/Layer.h"
#include "Clove/Events/Event.h"
#include "Clove/Events/ApplicationEvent.h"

#include "Clove/ImGui/ImGuiLayer.h"

namespace clv{
	class Layer;
	class Event;
	class WindowCloseEvent;

	class CLV_API Application{
		//VARIABLES
	private:
		std::unique_ptr<Window> window;
		std::shared_ptr<ImGuiLayer> imGuiLayer;

		bool running = true;
		LayerStack layerStack;

		static Application* instance;
		
		//FUNCTIONS
	public:
		Application();
		virtual ~Application() = default;

		void run();

		void onEvent(Event& e);

		void pushLayer(std::shared_ptr<Layer> layer);
		void pushOverlay(std::shared_ptr<Layer> overlay);

		inline static Application& get(){ return *instance; }
		inline Window& getWindow(){ return *window; }

	private:
		bool onWindowClose(WindowCloseEvent& e);
	};

	//To be defined in CLIENT
	Application* createApplication();
}

