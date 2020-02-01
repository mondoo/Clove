#include "Clove/Platform/Mac/MacWindow.hpp"

#include "Clove/Graphics/Core/GraphicsFactory.hpp"
#include "Clove/Graphics/Metal/MTLSurface.hpp"

@implementation MacWindowProxy
- (instancetype)initWithWindowData:(MTKView*)view width: (unsigned int)width height:(unsigned int)height name:(NSString*)name{
	const NSRect rect = NSMakeRect(0, 0, width, height);
	const NSWindowStyleMask styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable;
	
	_window = [[NSWindow alloc] initWithContentRect:rect
										  styleMask:styleMask
											backing:NSBackingStoreBuffered
											  defer:NO];

	[_window setTitle:name];
	[_window setDelegate:self];
	[_window makeKeyAndOrderFront:nil];

	[_window setContentView:view];
	
	return self;
}

- (instancetype)initWithParentWindow:(MTKView*)view parentWindow:(const clv::plt::Window&)parentWindow position:(const clv::mth::vec2i&)position size:(const clv::mth::vec2i&)size{
	const NSRect rect = NSMakeRect(position.x, position.y, size.x, size.y);
	const NSWindowStyleMask styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable;
	
	_window = [[NSWindow alloc] initWithContentRect:rect
										 styleMask:styleMask
										   backing:NSBackingStoreBuffered
											 defer:NO];

	[_window setDelegate:self];
	[_window makeKeyAndOrderFront:nil];
	
	[_window setContentView:view];

	NSWindow* nativeParentWindow = reinterpret_cast<NSWindow*>(parentWindow.getNativeWindow());
	[nativeParentWindow addChildWindow:_window ordered:NSWindowAbove];
	
	return self;
}

- (void)windowWillClose:(NSNotification *)notification{
	//The application shouldn't shut down when a window closes on MacOS, but this'll do for now
	_cloveWindow->onWindowCloseDelegate.broadcast();
}
@end

namespace clv::plt{
    MacWindow::MacWindow(gfx::GraphicsFactory& graphicsFactory, const WindowDescriptor& props){
		MacData data = { { props.width, props.height } };
		
		surface = graphicsFactory.createSurface(&data);
		
		NSString* nameString = [NSString stringWithCString:props.title.c_str() encoding:[NSString defaultCStringEncoding]];
		
		windowProxy = [[MacWindowProxy alloc] initWithWindowData:std::static_pointer_cast<gfx::mtl::MTLSurface>(surface)->getMTKView()
														   width:props.width
														  height:props.height
															name:nameString];
		windowProxy.cloveWindow = this;
    }
	
	MacWindow::MacWindow(gfx::GraphicsFactory& graphicsFactory, const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size){
		MacData data = { { size.x, size.y } };
		
		surface = graphicsFactory.createSurface(&data);
		
		windowProxy = [[MacWindowProxy alloc] initWithParentWindow:std::static_pointer_cast<gfx::mtl::MTLSurface>(surface)->getMTKView()
													  parentWindow:parentWindow
														  position:position
															  size:size];
		windowProxy.cloveWindow = this;
	}

	MacWindow::~MacWindow(){
		[windowProxy release];
	}
	
	void* MacWindow::getNativeWindow() const{
		return [windowProxy window];
	}
	
	mth::vec2i MacWindow::getPosition() const{
		NSRect frame = [[windowProxy window] frame];
		return { frame.origin.x, frame.origin.y };
	}
	
	mth::vec2i MacWindow::getSize() const{
		NSRect frame = [[windowProxy window] frame];
		return { frame.size.width, frame.size.height };
	}

	void MacWindow::moveWindow(const mth::vec2i& position){
		const mth::vec2i size = getSize();
		[[windowProxy window] setFrame:NSMakeRect(position.x, position.x, size.x, size.y) display:YES];
	}
	
	void MacWindow::resizeWindow(const mth::vec2i& size){
		const mth::vec2i position = getPosition();
		[[windowProxy window] setFrame:NSMakeRect(position.x, position.x, size.x, size.y) display:YES];
	}
	
	void MacWindow::processInput(){
		@autoreleasepool {
			NSEvent* event;
			do{
				event = [NSApp nextEventMatchingMask:NSEventMaskAny
										   untilDate:nil
											  inMode:NSDefaultRunLoopMode
											 dequeue:YES];
			
				mth::vec<2, int32, mth::qualifier::defaultp> mouseLoc{ static_cast<int32>([NSEvent mouseLocation].x), static_cast<int32>([NSEvent mouseLocation].y) };
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
						
					default:
						[NSApp sendEvent:event];
						break;
				}
			} while (event != nil);
		}
	}
}
