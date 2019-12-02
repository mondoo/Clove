#include "BulbApplication.hpp"

#include "BulbWindow.hpp"

namespace Clove{
	BulbApplication::BulbApplication(System::IntPtr hWnd, int width, int height)
		: width(width), height(height){
		this->hWnd = hWnd;
	}

	clv::gfx::API BulbApplication::getPlatformPreferedAPI(){
		return clv::gfx::API::DirectX11;
	}

	std::unique_ptr<clv::plt::Window> BulbApplication::createWindow(const clv::plt::WindowProps& props){
		return std::make_unique<BulbWindow>(hWnd, width, height);
	}
}