#include "Clove/Platform/Mac/MacWindow.hpp"

@implementation MacWindowProxy
- (instancetype)initWithWindowData:(NSView*)view width: (unsigned int)width height:(unsigned int)height name:(NSString*)name{
	NSRect const rect{ NSMakeRect(0, 0, width, height) };
	NSWindowStyleMask const styleMask{ NSWindowStyleMaskHUDWindow | NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable };
	
	_window = [[NSWindow alloc] initWithContentRect:rect
										  styleMask:styleMask
											backing:NSBackingStoreBuffered
											  defer:NO];

	[_window setTitle:name];
	[_window setDelegate:self];
	[_window setBackgroundColor:NSColor.redColor];
	[_window makeKeyAndOrderFront:nil];

	//[_window setContentView:view];
	
	return self;
}

- (void)windowWillClose:(NSNotification *)notification{
	//The application shouldn't shut down when a window closes on MacOS, but this'll do for now
	_cloveWindow->close();
}
@end

namespace garlic::clove{
    MacWindow::MacWindow(const WindowDescriptor& descriptor)
        : Window(keyboardDispatcher, mouseDispatcher) {
		NSString* nameString = [NSString stringWithCString:descriptor.title.c_str() encoding:[NSString defaultCStringEncoding]];
		
		//TODO: Retrieve MTLView from GraphicsDevice on initialisation
		windowProxy = [[MacWindowProxy alloc] initWithWindowData:nullptr
														   width:descriptor.width
														  height:descriptor.height
															name:nameString];
		windowProxy.cloveWindow = this;

		open = true;
    }

	MacWindow::~MacWindow(){
		[windowProxy release];
	}
	
	std::any MacWindow::getNativeWindow() const{
		return [windowProxy window];
	}
	
	vec2i MacWindow::getPosition() const{
		NSRect frame = [[windowProxy window] frame];
		return { frame.origin.x, frame.origin.y };
	}
	
	vec2i MacWindow::getSize() const{
		NSRect frame = [[windowProxy window] frame];
		return { frame.size.width, frame.size.height };
	}

	void MacWindow::moveWindow(const vec2i& position){
		const vec2i size = getSize();
		[[windowProxy window] setFrame:NSMakeRect(position.x, position.x, size.x, size.y) display:YES];
	}
	
	void MacWindow::resizeWindow(const vec2i& size){
		const vec2i position = getPosition();
		[[windowProxy window] setFrame:NSMakeRect(position.x, position.x, size.x, size.y) display:YES];
	}
	
	bool MacWindow::isOpen() const {
		return open;
	}

	void MacWindow::close() {
		if(onWindowCloseDelegate.isBound()) {
			onWindowCloseDelegate.broadcast();
		}
		open = false;
	}

	void MacWindow::processInput(){
		@autoreleasepool {
			while(NSEvent* event = [NSApp nextEventMatchingMask:NSEventMaskAny untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES]){
				vec2i mouseLoc{ static_cast<int32_t>([NSEvent mouseLocation].x), static_cast<int32_t>([NSEvent mouseLocation].y) };
				switch ([event type]){
					case NSEventTypeKeyDown:
						keyboardDispatcher.onKeyPressed(static_cast<Key>([event keyCode]));
						break;
					
					case NSEventTypeKeyUp:
						keyboardDispatcher.onKeyReleased(static_cast<Key>([event keyCode]));
						break;
					
					//TODO: Is there a char or key typed event?
				
					case NSEventTypeMouseEntered:
						mouseDispatcher.onMouseEnter();
						break;
					
					case NSEventTypeMouseExited:
						mouseDispatcher.onMouseLeave();
						break;
					
					case NSEventTypeMouseMoved:
						mouseDispatcher.onMouseMove(mouseLoc.x, mouseLoc.y);
						break;
					
					case NSEventTypeLeftMouseDown:
						mouseDispatcher.onButtonPressed(MouseButton::_1, mouseLoc.x, mouseLoc.y);
						break;
					
					case NSEventTypeLeftMouseUp:
						mouseDispatcher.onButtonReleased(MouseButton::_1, mouseLoc.x, mouseLoc.y);
						break;
					
					case NSEventTypeRightMouseDown:
						mouseDispatcher.onButtonPressed(MouseButton::_2, mouseLoc.x, mouseLoc.y);
						break;
					
					case NSEventTypeRightMouseUp:
						mouseDispatcher.onButtonReleased(MouseButton::_2, mouseLoc.x, mouseLoc.y);
						break;
					
					case NSEventTypeOtherMouseDown:
						if(([NSEvent pressedMouseButtons] & static_cast<NSUInteger>(MouseButton::_3)) != 0){
							mouseDispatcher.onButtonPressed(MouseButton::_3, mouseLoc.x, mouseLoc.y);
						}else if(([NSEvent pressedMouseButtons] & static_cast<NSUInteger>(MouseButton::_4)) != 0){
							mouseDispatcher.onButtonPressed(MouseButton::_4, mouseLoc.x, mouseLoc.y);
						}else if(([NSEvent pressedMouseButtons] & static_cast<NSUInteger>(MouseButton::_5)) != 0){
							mouseDispatcher.onButtonPressed(MouseButton::_5, mouseLoc.x, mouseLoc.y);
						}
						break;
					
					case NSEventTypeOtherMouseUp:
						if(([NSEvent pressedMouseButtons] & static_cast<NSUInteger>(MouseButton::_3)) != 0){
							mouseDispatcher.onButtonReleased(MouseButton::_3, mouseLoc.x, mouseLoc.y);
						}else if(([NSEvent pressedMouseButtons] & static_cast<NSUInteger>(MouseButton::_4)) != 0){
							mouseDispatcher.onButtonReleased(MouseButton::_4, mouseLoc.x, mouseLoc.y);
						}else if(([NSEvent pressedMouseButtons] & static_cast<NSUInteger>(MouseButton::_5)) != 0){
							mouseDispatcher.onButtonReleased(MouseButton::_5, mouseLoc.x, mouseLoc.y);
						}
						break;
					
					case NSEventTypeScrollWheel:
						mouseDispatcher.onWheelDelta(static_cast<int32_t>([event scrollingDeltaY]), mouseLoc.x, mouseLoc.y);
						break;
						
					default:
						[NSApp sendEvent:event];
						break;
				}
			}
		}
	}
}
