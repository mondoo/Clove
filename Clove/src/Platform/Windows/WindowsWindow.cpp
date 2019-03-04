#include "clvpch.h"
#include "WindowsWindow.h"

namespace clv{
	static bool GLFWInitialised = false;

	Window* Window::create(const WindowProps& props){
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props){
		init(props);
	}

	WindowsWindow::~WindowsWindow(){
		shutdown();
	}

	void WindowsWindow::onUpdate(){
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	void WindowsWindow::setVSync(bool enabled){
		glfwSwapInterval(enabled ? 1 : 0);
		data.vSync = true;
	}

	bool WindowsWindow::isVSync() const{
		return data.vSync;
	}

	void WindowsWindow::init(const WindowProps& props){
		data.title = props.title;
		data.width = props.width;
		data.height = props.height;

		CLV_CORE_INFO("Creating window {0} ({1}, {2})", data.title, data.width, data.height);

		if(!GLFWInitialised){
			//TODO: glfwTerminate on system shutdown
			const int success = glfwInit();
			CLV_CORE_ASSERT(success, "Could not initialise GLFW!");

			GLFWInitialised = true;
		}

		window = glfwCreateWindow(static_cast<int>(data.width), static_cast<int>(data.height), data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, &data);
		setVSync(true);
	}

	void WindowsWindow::shutdown(){
		glfwDestroyWindow(window);
	}
}