#include "clvpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
//Our hacked renderer
#include "Platform/OpenGL/imgui_impl_opengl3.h"
//Keyevents
#include "Platform/OpenGL/imgui_impl_glfw.h"
//Temporary
#include <GLFW/glfw3.h>

#include "Clove/Application.h"

namespace clv{
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer"){

	}

	ImGuiLayer::~ImGuiLayer(){

	}

	void ImGuiLayer::onAttach(){
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		////TEMPORARY: Should eventually use Clove keycodes
		//// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
		//io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		//io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		//io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		//io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		//io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		//io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		//io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		//io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		//io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		//io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		//io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		//io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		//io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		//io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		//io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		//io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		//io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		//io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		//io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		//io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		//io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		Application& app = Application::get();
		Window& window = app.getWindow();

		this->window = static_cast<GLFWwindow*>(window.getWindowObject());

		CLV_CORE_ASSERT(this->window, "Window object is null!");

		ImGui_ImplGlfw_InitForOpenGL(this->window, false);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::onDetach(){
		window = nullptr;
	}

	void ImGuiLayer::onUpdate(){
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::get();
		io.DisplaySize = ImVec2(app.getWindow().getWidth(), app.getWindow().getHeight());

		float time = static_cast<float>(glfwGetTime());
		io.DeltaTime = this->time > 0.0f ? (time - this->time) : (1.0f / 60.0f);
		this->time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::onEvent(Event& e){
		EventDispatcher dispatcher(e);
		
		dispatcher.dispatch<MouseButtonPressedEvent>(BIND_FUNCTION_OneParam(&ImGuiLayer::onMouseButtonPressed, this));
		dispatcher.dispatch<MouseScrolledEvent>(BIND_FUNCTION_OneParam(&ImGuiLayer::onMouseScrolled, this));
		dispatcher.dispatch<KeyPressedEvent>(BIND_FUNCTION_OneParam(&ImGuiLayer::onKeyPressed, this));
	}

	bool ImGuiLayer::onMouseButtonPressed(MouseButtonPressedEvent& e){
		ImGui_ImplGlfw_MouseButtonCallback(window, e.getMouseButton(), GLFW_PRESS, 0);
		return true;
	}

	bool ImGuiLayer::onMouseButtonReleased(MouseButtonReleasedEvent& e){
		ImGui_ImplGlfw_MouseButtonCallback(window, e.getMouseButton(), GLFW_RELEASE, 0);
		return true;
	}

	bool ImGuiLayer::onMouseScrolled(MouseScrolledEvent & e){
		ImGui_ImplGlfw_ScrollCallback(window, static_cast<double>(e.getXOffset()), static_cast<double>(e.getYOffset()));
		return true;
	}

	bool ImGuiLayer::onKeyPressed(KeyPressedEvent& e){
		ImGui_ImplGlfw_KeyCallback(window, e.getKeyCode(), 0, GLFW_PRESS, 0);
		return true;
	}

	bool ImGuiLayer::onKeyReleased(KeyReleasedEvent& e){
		ImGui_ImplGlfw_KeyCallback(window, e.getKeyCode(), 0, GLFW_RELEASE, 0);
		return true;
	}
}