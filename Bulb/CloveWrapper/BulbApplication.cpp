#include "BulbApplication.hpp"

#include "BulbWindow.hpp"

namespace clv::plt::blb{
	BulbNativeApplication::BulbNativeApplication(System::IntPtr hWnd, int width, int height)
		: width(width), height(height){
		this->hWnd = hWnd;
	}

	clv::gfx::API BulbNativeApplication::getPlatformPreferedAPI(){
		return clv::gfx::API::DirectX11;
	}

	std::unique_ptr<clv::plt::Window> BulbNativeApplication::createWindow(const clv::plt::WindowProps& props){
		return std::make_unique<BulbNativeWindow>(hWnd, width, height);
	}
}

namespace Bulb::CloveWrapper{
	BulbApplication::BulbApplication(System::IntPtr hWnd, int width, int height){
		nativeApp = new clv::plt::blb::BulbNativeApplication(hWnd, width, height);
	}

	BulbApplication::~BulbApplication(){
		delete nativeApp;
	}

	void BulbApplication::start(){
		nativeApp->start();
	}

	void BulbApplication::update(){
		nativeApp->update();
	}

	void BulbApplication::stop(){
		nativeApp->stop();
	}
	
	bool BulbApplication::shouldRun(){
		return nativeApp->getState() != clv::plt::ApplicationState::stopped;
	}
}