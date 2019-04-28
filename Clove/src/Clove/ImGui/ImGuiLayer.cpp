#include "clvpch.hpp"
#include "ImGuiLayer.hpp"

#include "Clove/Application.hpp"
#include "Clove/Window.hpp"
#include "Clove/Events/MouseEvent.hpp"
#include "Clove/Events/KeyEvent.hpp"
#include "Clove/Events/ApplicationEvent.hpp"

#include <imgui.h>
#include <GLFW/glfw3.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

namespace clv{
	ImGuiLayer::ImGuiLayer() 
		: Layer("ImGuiLayer"){

	}

	ImGuiLayer::~ImGuiLayer(){

	}

	void ImGuiLayer::onAttach(){
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsMerge;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		//TODO: Init for opengl / directed 11 with Wn32 API
		/*Application& app = Application::get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");*/
	}

	void ImGuiLayer::onDetach(){
		//TODO: shutdown for opengl / directed 11 with Wn32 API
		/*ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();*/
		ImGui::DestroyContext();
	}

	void ImGuiLayer::onImGuiRender(){

	}

	void ImGuiLayer::begin(){
		//TODO: opengl / directed 11 with Wn32 API
		/*ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();*/
		ImGui::NewFrame();
	}

	void ImGuiLayer::end(){
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::get();
		io.DisplaySize = ImVec2(static_cast<float>(app.getWindow().getWidth()), static_cast<float>(app.getWindow().getHeight()));

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}