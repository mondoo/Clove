#import "Platform/Mac/CloveMac.h"
#import "MacWindow.hpp"

#import <AppKit/AppKit.h>

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
		//TODO: Process input events (here or with a window delegate)
		NSEvent* event;
		do{
			event = [NSApp nextEventMatchingMask:NSEventMaskAny
									   untilDate:nil
										  inMode:NSDefaultRunLoopMode
										 dequeue:YES];
			
			math::Vector<2, int32, math::qualifier::defaultp> mouseLoc{ static_cast<int32>([NSEvent mouseLocation].x), static_cast<int32>([NSEvent mouseLocation].y) };
			switch ([event type]){
				//TODO: Close
				
				case NSEventTypeKeyDown:
					keyboard.onKeyPressed(static_cast<Key>([event keyCode]));
					break;
					
				case NSEventTypeKeyUp:
					keyboard.onKeyReleased(static_cast<Key>([event keyCode]));
					break;
					
				//TODO: Char
				
				case NSEventTypeMouseEntered:
					mouse.onMouseEnter();
					break;
					
				case NSEventTypeMouseExited:
					mouse.onMouseLeave();
					break;
					
				case NSEventTypeMouseMoved:
					mouse.onMouseMove(mouseLoc.x, mouseLoc.y);
					break;
					
				case NSEventTypeLeftMouseDown:
					mouse.onButtonPressed(MouseButton::_1, mouseLoc.x, mouseLoc.y);
					break;
					
				case NSEventTypeLeftMouseUp:
					mouse.onButtonReleased(MouseButton::_1, mouseLoc.x, mouseLoc.y);
					break;
					
				case NSEventTypeRightMouseDown:
					mouse.onButtonPressed(MouseButton::_2, mouseLoc.x, mouseLoc.y);
					break;
					
				case NSEventTypeRightMouseUp:
					mouse.onButtonReleased(MouseButton::_2, mouseLoc.x, mouseLoc.y);
					break;
					
				case NSEventTypeOtherMouseDown:
					if(([NSEvent pressedMouseButtons] & static_cast<NSUInteger>(MouseButton::_3)) != 0){
						mouse.onButtonPressed(MouseButton::_3, mouseLoc.x, mouseLoc.y);
					}else if(([NSEvent pressedMouseButtons] & static_cast<NSUInteger>(MouseButton::_4)) != 0){
						mouse.onButtonPressed(MouseButton::_4, mouseLoc.x, mouseLoc.y);
					}else if(([NSEvent pressedMouseButtons] & static_cast<NSUInteger>(MouseButton::_5)) != 0){
						mouse.onButtonPressed(MouseButton::_5, mouseLoc.x, mouseLoc.y);
					}
					break;
					
				case NSEventTypeOtherMouseUp:
					if(([NSEvent pressedMouseButtons] & static_cast<NSUInteger>(MouseButton::_3)) != 0){
						mouse.onButtonReleased(MouseButton::_3, mouseLoc.x, mouseLoc.y);
					}else if(([NSEvent pressedMouseButtons] & static_cast<NSUInteger>(MouseButton::_4)) != 0){
						mouse.onButtonReleased(MouseButton::_4, mouseLoc.x, mouseLoc.y);
					}else if(([NSEvent pressedMouseButtons] & static_cast<NSUInteger>(MouseButton::_5)) != 0){
						mouse.onButtonReleased(MouseButton::_5, mouseLoc.x, mouseLoc.y);
					}
					break;
					
				case NSEventTypeScrollWheel:
					mouse.onWheelDelta(static_cast<int32>([event scrollingDeltaY]), mouseLoc.x, mouseLoc.y);
					break;
					
				default:
					[NSApp sendEvent: event];
					break;
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
