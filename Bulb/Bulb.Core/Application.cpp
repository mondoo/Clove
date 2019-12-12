#include "Application.hpp"

#include "Window.hpp"
#include "Layer.hpp"

#include <Clove/Core/ECS/Manager.hpp>

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

namespace Bulb::Core{
	Application::Application(System::IntPtr hWnd, int width, int height){
		nativeApp = new clv::plt::blb::NativeApplication(hWnd, width, height);
	}

	Application::!Application(){
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
		nativeApp->pushLayer(std::make_shared<clv::blb::NativeLayer>(layer));
	}
	
	Entity^ Application::createEntity(){
		auto clvEnt = clv::plt::Application::get().getManager().createEntity();
		return gcnew Entity(new clv::ecs::blb::NativeEntity(clvEnt.getID(), &clv::plt::Application::get().getManager()));
	}
}