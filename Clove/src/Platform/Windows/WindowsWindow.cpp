#include "clvpch.h"
#include "WindowsWindow.h"

#include "Clove/Events/ApplicationEvent.h"
#include "Clove/Events/MouseEvent.h"
#include "Clove/Events/KeyEvent.h"

#include "Clove/Rendering/Renderer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace clv{
	static bool GLFWInitialised = false;

	static void GFLWErrorCallback(int error, const char* description){
		CLV_ERROR("GLFW Error ({0}): {1}", error, description);
	}

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

	inline unsigned int WindowsWindow::getWidth() const{
		return data.width;
	}

	inline unsigned int WindowsWindow::getHeight() const{
		return data.height;
	}

	inline void WindowsWindow::setEventCallbackFunction(const EventCallbackFn& callback){
		data.eventCallback = callback;
	}

	void WindowsWindow::setVSync(bool enabled){
		glfwSwapInterval(enabled ? 1 : 0);
		data.vSync = enabled;
	}

	bool WindowsWindow::isVSync() const{
		return data.vSync;
	}

	void* WindowsWindow::getNativeWindow() const{
		return window;
	}

	const Renderer& WindowsWindow::getRenderer() const{
		return renderer;
	}

	void WindowsWindow::init(const WindowProps& props){
		data.title = props.title;
		data.width = props.width;
		data.height = props.height;

		CLV_TRACE("Creating window: {0} ({1}, {2})", data.title, data.width, data.height);

		if(!GLFWInitialised){
			//TODO: glfwTerminate on system shutdown
			const int success = glfwInit();
			CLV_ASSERT(success, "Could not initialise GLFW!");
			glfwSetErrorCallback(GFLWErrorCallback);
			GLFWInitialised = true;
		}

		//setting opengl to 3.3.0 core profile
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Set the core profile (basically stricter and does less for us)
		//

		window = glfwCreateWindow(static_cast<int>(data.width), static_cast<int>(data.height), data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window);

		int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		CLV_ASSERT(status, "Failed to initialise Glad");

		CLV_INFO("Window created!");
		CLV_INFO("GL version: {0}", glGetString(GL_VERSION));

		CLV_TRACE("Enabling Depth buffer");
		GLCall(glDepthFunc(GL_LESS));
		GLCall(glEnable(GL_DEPTH_TEST));

		CLV_TRACE("Blend set to: SRC_ALPHA | ONE_MINUS_SRC_ALPHA");
		//src is from the image - dest is what is already in the buffer
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));
		//I guess it's called blending because you blend the src with the destination

		glfwSetWindowUserPointer(window, &data);
		setVSync(true);

		//Set GLFW callbacks
		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height){
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);
			data.eventCallback(event);
		});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window){
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			WindowCloseEvent event;
			data.eventCallback(event);
		});

		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch(action){
			case GLFW_PRESS:
			{
				KeyPressedEvent event(KeyCodeHelpers::GLFWToClove(key), 0);
				data.eventCallback(event);
				break;
			}

			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(KeyCodeHelpers::GLFWToClove(key));
				data.eventCallback(event);
				break;
			}

			case GLFW_REPEAT:
			{
				KeyPressedEvent event(KeyCodeHelpers::GLFWToClove(key), 1); //repeat is manually put to 1 for now
				data.eventCallback(event);
				break;
			}
			}
		});

		glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int character){
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			KeyTypedEvent event(KeyCodeHelpers::GLFWToClove(static_cast<int>(character)));
			data.eventCallback(event);
		});

		glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods){
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch(action){
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(MouseButtonCodeHelpers::GLFWToClove(button));
				data.eventCallback(event);
				break;
			}

			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(MouseButtonCodeHelpers::GLFWToClove(button));
				data.eventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset){
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
			data.eventCallback(event);
		});

		glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos){
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
			data.eventCallback(event);
		});
	}

	void WindowsWindow::shutdown(){
		glfwDestroyWindow(window);
		CLV_INFO("Window destroyed");
	}
}