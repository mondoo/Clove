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
		return window;
	}
	
	void MacWindow::processInput(){
		[window isVisible];
		
		//TODO: Process input events (here or with a window delegate)
		NSEvent* event;
		do {
			event = [NSApp nextEventMatchingMask:NSEventMaskAny
									   untilDate:nil
										  inMode:NSDefaultRunLoopMode
										 dequeue:YES];
			
			switch ([event type]) {
				default:
				[NSApp sendEvent: event];
			}
		} while (event != nil);
	}
	
	void MacWindow::initialiseWindow(const WindowProps& props, gfx::API api){
		windowProperties.width = props.width;
		windowProperties.height = props.height;
		windowProperties.title = props.title;
		
		NSRect rect = NSMakeRect(100, 100, props.width, props.height);
		NSWindowStyleMask styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable;
		
		window = [[NSWindow alloc] initWithContentRect:rect
											 styleMask:styleMask
											   backing:NSBackingStoreBuffered
												 defer:NO];
		[window setBackgroundColor:NSColor.redColor];
		[window setTitle:[NSString stringWithCString:props.title.c_str() encoding:[NSString defaultCStringEncoding]]];
		[window makeKeyAndOrderFront:nil];
		
		//Pack data and send to context
		
	}

    Window* Window::create(const WindowProps& props){
        return new MacWindow(props);
    }

    Window* Window::create(const WindowProps& props, gfx::API api){
        return new MacWindow(props, api);
    }
}
