#import "Platform/Mac/CloveMac.h"
#import "MacWindow.hpp"

#import "Clove/Graphics/Renderer.hpp"

namespace clv {
	MacWindow::~MacWindow(){
		//[window release];
	}

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
		CLV_LOG_DEBUG("here");
		
		//NOTE: The below works when running the executable manually (not through xcode)
		
		NSRect rect = NSMakeRect(100, 100, 800, 600);
		NSWindowStyleMask styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable;
		
		window = [[NSWindow alloc] initWithContentRect:rect styleMask:styleMask backing:NSBackingStoreBuffered defer:NO];
		[window setBackgroundColor:NSColor.redColor];
		[window setTitle:@"CLOVE!!!!"];
		[window makeKeyAndOrderFront:nil];
		
		//What will become provess input
		while(true) {
			NSEvent* event;
			do { //This will be in process input
				event = [NSApp nextEventMatchingMask: NSEventMaskAny untilDate: nil inMode: NSDefaultRunLoopMode dequeue: YES];
				
				switch ([event type]) {
					default:
					[NSApp sendEvent: event];
				}
			} while (event != nil);}
		
		CLV_LOG_DEBUG("Finished");
	}

    Window* Window::create(const WindowProps& props){
        return new MacWindow(props);
    }

    Window* Window::create(const WindowProps& props, gfx::API api){
        return new MacWindow(props, api);
    }
}
