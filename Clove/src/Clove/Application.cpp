#include "clvpch.h"
#include "Application.h"

#include "Clove/Events/ApplicationEvent.h"
#include "Clove/Log.h"

//TODO: Remove
#include <GLFW/glfw3.h>

namespace clv{
	Application::Application(){
		window = std::unique_ptr<Window>(Window::create());
	}

	void Application::run(){
		while(running){
			//TODO: Remove
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			//

			window->onUpdate();
		}
	}
}