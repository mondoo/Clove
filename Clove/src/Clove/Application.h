#pragma once

#include "Core.h"

#include "Window.h"
#include "Clove/LayerStack.h"
#include "Clove/Layer.h"
#include "Clove/Events/Event.h"
#include "Clove/Events/ApplicationEvent.h"

namespace clv{
	class Layer;
	class Event;
	class WindowCloseEvent;

	class CLV_API Application{
		//VARIABLES
	private:
		std::unique_ptr<Window> window;
		bool running = true;
		LayerStack layerStack;
		
		//FUNCTIONS
	public:
		Application();
		virtual ~Application() = default;

		void run();

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

	private:
		bool onWindowClose(WindowCloseEvent& e);
	};

	//To be defined in CLIENT
	Application* createApplication();
}

