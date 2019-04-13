#include "clvpch.hpp"
#include "Window.hpp"
#include "Clove/Events/Event.hpp"

namespace clv{
	Camera& Window::getCurrentCamera(){
		return camera;
	}
}