#include "clvpch.hpp"
#include "ImGuiLayer.hpp"

#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Events/MouseEvent.hpp"
#include "Clove/Events/KeyEvent.hpp"
#include "Clove/Events/ApplicationEvent.hpp"
#if CLV_PLATFORM_WINDOWS
#include "Graphics/DirectX-11/DX11Renderer.hpp"
#endif

#include <imgui.h>
#include <examples/imgui_impl_opengl3.h>
#if CLV_PLATFORM_WINDOWS
#include <examples/imgui_impl_dx11.h>>
#include <examples/imgui_impl_win32.h>
#endif

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

		currentAPI = Application::get().getRenderer().getAPI();

		switch(currentAPI){
			case gfx::API::OpenGL4:
				ImGui_ImplOpenGL3_Init("#version 430");
				break;

			#if CLV_PLATFORM_WINDOWS
			case gfx::API::DirectX11:
				if(gfx::DX11Renderer* dxrenderer = dynamic_cast<gfx::DX11Renderer*>(&Application::get().getRenderer())){
					ImGui_ImplDX11_Init(&dxrenderer->getDevice(), &dxrenderer->getContext());
				}
				break;
			#endif

			default:
				break;
		}

	#if CLV_PLATFORM_WINDOWS
		ImGui_ImplWin32_Init(static_cast<HWND>(Application::get().getWindow().getNativeWindow()));
	#endif
	}

	void ImGuiLayer::onDetach(){
		switch(currentAPI){
			case gfx::API::OpenGL4:
				ImGui_ImplOpenGL3_Shutdown();
				break;

			#if CLV_PLATFORM_WINDOWS
			case gfx::API::DirectX11:
				ImGui_ImplDX11_Shutdown();
				break;
			#endif

			default:
				break;
		}

	#if CLV_PLATFORM_WINDOWS
		ImGui_ImplWin32_Shutdown();
	#endif

		ImGui::DestroyContext();
	}

	void ImGuiLayer::onImGuiRender(){

	}

	void ImGuiLayer::begin(){
		switch(currentAPI){
			case gfx::API::OpenGL4:
				ImGui_ImplOpenGL3_NewFrame();
				break;

			#if CLV_PLATFORM_WINDOWS
			case gfx::API::DirectX11:
				ImGui_ImplDX11_NewFrame();
				break;
			#endif

			default:
				break;
		}

	#if CLV_PLATFORM_WINDOWS
		ImGui_ImplWin32_NewFrame();
	#endif

		ImGui::NewFrame();
	}

	void ImGuiLayer::end(){
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::get();
		io.DisplaySize = ImVec2(static_cast<float>(app.getWindow().getWidth()), static_cast<float>(app.getWindow().getHeight()));

		ImGui::Render();

		switch(currentAPI){
			case gfx::API::OpenGL4:
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
				break;

			#if CLV_PLATFORM_WINDOWS
			case gfx::API::DirectX11:
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
				break;
			#endif

			default:
				break;
		}

	#if CLV_PLATFORM_WINDOWS
		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	#endif
	}
}