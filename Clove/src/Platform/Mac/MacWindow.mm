#import "Platform/Mac/CloveMac.h"
#import "MacWindow.hpp"

#import "Clove/Graphics/Renderer.hpp"

namespace clv {
    MacWindow::~MacWindow() = default;

    MacWindow::MacWindow(const WindowProps& props){
		initialiseWindow(props, gfx::API::OpenGL4);
    }
    
    MacWindow::MacWindow(const WindowProps& props, gfx::API api){
		initialiseWindow(props, api);
    }
	
	void* MacWindow::getNativeWindow() const{
		//TODO
		return nullptr;
	}
	
	void MacWindow::processInput(){
		//TODO
	}
	
	void MacWindow::initialiseWindow(const WindowProps& props, gfx::API api){
		window = [[NSWindow alloc] init];
		controller = [[NSWindowController alloc] init];
		
		[window initWithContentRect:NSMakeRect(100, 100, 800, 600) styleMask:NSWindowStyleMaskTitled backing:NSBackingStoreBuffered defer:false screen:nil];
		
		[controller initWithWindow:window];
		[controller showWindow:nil]; //TODO: Does not open
		
		//[window makeKeyAndOrderFront:nil];
	}

    Window* Window::create(const WindowProps& props){
        return new MacWindow(props);
    }

    Window* Window::create(const WindowProps& props, gfx::API api){
        return new MacWindow(props, api);
    }
}
