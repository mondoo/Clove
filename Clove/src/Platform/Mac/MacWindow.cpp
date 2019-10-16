#include "MacWindow.hpp"

namespace clv {
    MacWindow::~MacWindow() = default;

    MacWindow::MacWindow(const WindowProps& props){
        //TODO
    }
    
    MacWindow::MacWindow(const WindowProps& props, gfx::API api){
        //TODO
    }
	
	void* MacWindow::getNativeWindow() const{
		//TODO
		return nullptr;
	}
	
	void MacWindow::processInput(){
		//TODO
	}

    Window* Window::create(const WindowProps& props){
        return new MacWindow(props);
    }

    Window* Window::create(const WindowProps& props, gfx::API api){
        return new MacWindow(props, api);
    }
}
