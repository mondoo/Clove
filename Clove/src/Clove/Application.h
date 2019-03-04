#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Clove/Events/ApplicationEvent.h"

#include "Window.h"

namespace clv{
	class CLV_API Application{
		//VARIABLES
	private:
		std::unique_ptr<Window> window;

		bool running = true;
		
		//FUNCTIONS
	public:
		Application();
		virtual ~Application() = default;

		void run();

		void onEvent(Event& e);

	private:
		bool onWindowClose(WindowCloseEvent& e);
	};

	//To be defined in CLIENT
	Application* createApplication();
}

