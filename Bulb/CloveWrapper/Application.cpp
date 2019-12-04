#include "Application.hpp"

#include "Window.hpp"

namespace clv::plt::blb{
	NativeApplication::NativeApplication(System::IntPtr hWnd, int width, int height)
		: width(width), height(height){
		this->hWnd = hWnd;
	}

	clv::gfx::API NativeApplication::getPlatformPreferedAPI(){
		return clv::gfx::API::DirectX11;
	}

	std::unique_ptr<clv::plt::Window> NativeApplication::createWindow(const clv::plt::WindowProps& props){
		return std::make_unique<NativeWindow>(hWnd, width, height);
	}
}

namespace Clove{
	Application::Application(System::IntPtr hWnd, int width, int height){
		nativeApp = new clv::plt::blb::NativeApplication(hWnd, width, height);
	}

	Application::~Application(){
		delete nativeApp;
	}

	void Application::start(){
		nativeApp->start();
	}

	void Application::update(){
		nativeApp->update();
	}

	void Application::stop(){
		nativeApp->stop();
	}
	
	bool Application::shouldRun(){
		return nativeApp->getState() != clv::plt::ApplicationState::stopped;
	}

	void Application::pushLayer(Layer^ layer){
		nativeApp->pushLayer(layer->getNativeLayer());
		layers.Add(layer);
		//NOTE: Potential here for them to become out of sync
	}
}