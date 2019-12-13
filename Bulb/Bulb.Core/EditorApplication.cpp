#include "EditorApplication.hpp"

#include "EditorWindow.hpp"
#include "EditorLayer.hpp"

#include <Clove/Core/ECS/Manager.hpp>

namespace clv::plt::blb{
	EditorApplication::EditorApplication(System::IntPtr hWnd, int width, int height)
		: width(width), height(height){
		this->hWnd = hWnd;
	}

	clv::gfx::API EditorApplication::getPlatformPreferedAPI(){
		return clv::gfx::API::DirectX11;
	}

	std::unique_ptr<clv::plt::Window> EditorApplication::createWindow(const clv::plt::WindowProps& props){
		return std::make_unique<EditorWindow>(hWnd, width, height);
	}
}