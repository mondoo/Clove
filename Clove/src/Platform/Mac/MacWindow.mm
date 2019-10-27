#import "Platform/Mac/CloveMac.h"
#import "MacWindow.hpp"

#import "Clove/Graphics/Renderer.hpp"
#import "Clove/Graphics/Context.hpp"

@implementation MacWindowProxy
- (instancetype)initWithWindowData:(unsigned int)width height:(unsigned int)height name:(NSString*)name{
	const NSRect rect = NSMakeRect(0, 0, width, height);
	const NSWindowStyleMask styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable;
	
	_window = [[NSWindow alloc] initWithContentRect:rect
										 styleMask:styleMask
										   backing:NSBackingStoreBuffered
											 defer:NO];

	[_window setTitle:name];
	[_window setDelegate:self];
	[_window makeKeyAndOrderFront:nil];
	
	return self;
}

- (void)windowWillClose:(NSNotification *)notification{
	//The application shouldn't shut down when a window closes on MacOS, but this'll do for now
	_cloveWindow->onWindowCloseDelegate.broadcast();
}
@end

namespace clv::plt{
	MacWindow::~MacWindow(){
		[windowProxy release];
	}

    MacWindow::MacWindow(const WindowProps& props){
		initialiseWindow(props, gfx::API::OpenGL4);
    }
    
    MacWindow::MacWindow(const WindowProps& props, gfx::API api){
		initialiseWindow(props, api);
    }
	
	void* MacWindow::getNativeWindow() const{
		return [windowProxy window];
	}
	
	void MacWindow::processInput(){
		@autoreleasepool {
			NSEvent* event;
			do{
				event = [NSApp nextEventMatchingMask:NSEventMaskAny
										   untilDate:nil
											  inMode:NSDefaultRunLoopMode
											 dequeue:YES];
			
				math::Vector<2, int32, math::qualifier::defaultp> mouseLoc{ static_cast<int32>([NSEvent mouseLocation].x), static_cast<int32>([NSEvent mouseLocation].y) };
				switch ([event type]){
					case NSEventTypeKeyDown:
						keyboard.onKeyPressed(static_cast<Key>([event keyCode]));
						break;
					
					case NSEventTypeKeyUp:
						keyboard.onKeyReleased(static_cast<Key>([event keyCode]));
						break;
					
					//TODO: Is there a char or key typed event?
				
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
				}
				[NSApp sendEvent: event];
			} while (event != nil);
		}
	}
	
	void MacWindow::initialiseWindow(const WindowProps& props, gfx::API api){		
		windowProperties.width = props.width;
		windowProperties.height = props.height;
		windowProperties.title = props.title;
		
		NSString* nameString = [NSString stringWithCString:props.title.c_str() encoding:[NSString defaultCStringEncoding]];
		
		windowProxy = [[MacWindowProxy alloc] initWithWindowData:props.width
															 height:props.height
															   name:nameString];
		windowProxy.cloveWindow = this;
	}
}
