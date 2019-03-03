#include "Application.h"

#include "Clove/Events/ApplicationEvent.h"
#include "Clove/Log.h"

namespace clv{
	void Application::run(){
		WindowResizeEvent e{ 1280, 720 };
		CLV_TRACE(e);

		while(true){

		}
	}
}