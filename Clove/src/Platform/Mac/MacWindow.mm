#import "Platform/Mac/CloveMac.h"
#import "MacWindow.hpp"

#import "Core/Graphics/Surface.hpp"
#import "Core/Graphics/RenderCommand.hpp"

//Temp metal stuff
//#import <Metal/Metal.h>
#import "Graphics/Metal/ShaderStrings.hpp"

//Metal constant buffer struct
struct constants{
	float animateBy = 0.5f;
};
//---

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
	
	_view = [[[MTKView alloc] initWithFrame:rect] autorelease]; //View could be the surface
	
	[_window setContentView:_view];
	
	//Metal stuff
	[_view setDelegate:self];
	[_view setDevice:MTLCreateSystemDefaultDevice()];
	_device = [_view device];
	
	[_view setClearColor:MTLClearColorMake(0.0, 0.4, 0.21, 1.0)];
	
	//TODO: Clove will need to support command queues / buffers
	_commandQueue = [_device newCommandQueue];
	//---
	
	return self;
}

- (void)windowWillClose:(NSNotification *)notification{
	//The application shouldn't shut down when a window closes on MacOS, but this'll do for now
	_cloveWindow->onWindowCloseDelegate.broadcast();
}

//MTKViewDelegate functions
- (void)mtkView:(MTKView *)view drawableSizeWillChange:(CGSize)size{
	//Empty for now
}

- (void)drawInMTKView:(MTKView *)view{
	static float time = 0.0f;
	static constants constant;
	
	float vertices[] = {
		-1,  1,  0,
		-1, -1,  0,
		 1, -1,  0,
		 1,  1,  0
	};
	
	UInt32 indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	
	time += 1.0f / [view preferredFramesPerSecond];
	constant.animateBy = abs(sin(time) / 2 + 0.5f);
	
	id<MTLDrawable> drawable = [view currentDrawable];
	MTLRenderPassDescriptor* descriptor = [view currentRenderPassDescriptor];
	
	//Create library to make our shaders
	NSError* error2 = [[NSError alloc] init];
	NSString* librarySource = [NSString stringWithCString:shader_Shader.c_str() encoding:[NSString defaultCStringEncoding]];
	id<MTLLibrary> library = [_device newLibraryWithSource:librarySource options:nil error:&error2];
	id<MTLFunction> vertexFunction = [library newFunctionWithName:@"vertex_shader"];
	id<MTLFunction> fragmentFunction = [library newFunctionWithName:@"fragment_shader"];
	
	//Set up the pipeline object
	MTLRenderPipelineDescriptor* pipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
	[pipelineDescriptor setVertexFunction:vertexFunction];
	[pipelineDescriptor setFragmentFunction:fragmentFunction];
	pipelineDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
	NSError *error = [[NSError alloc] init];
	id<MTLRenderPipelineState> pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineDescriptor error:&error];
	
	//Creating vertex buffer
	id<MTLBuffer> vertexBuffer = [_device newBufferWithBytes:vertices length:sizeof(vertices) options:0];
	
	//Creating index buffer
	id<MTLBuffer> indexBuffer = [_device newBufferWithBytes:indices length:sizeof(indices) options:0];
	
	//Creating constant buffer
	//id<MTLBuffer> constantBuffer = [_device newBufferWithBytes:&constant length:sizeof(constants) options:0];
	
	//Command queue / encoder
	id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
	id<MTLRenderCommandEncoder> commandEncoder = [commandBuffer renderCommandEncoderWithDescriptor:descriptor];
	
	//Render commands: start
	[commandEncoder setRenderPipelineState:pipelineState];
	
	[commandEncoder setVertexBuffer:vertexBuffer offset:0 atIndex:0]; //At index can be thought of as the binding point
	[commandEncoder setVertexBytes:&constant length:sizeof(constants) atIndex:1];
	
	[commandEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
							   indexCount:sizeof(indices)/sizeof(UInt32)
								indexType:MTLIndexTypeUInt32
							  indexBuffer:indexBuffer
						indexBufferOffset:0];
	//Render commands: end
	
	[commandEncoder endEncoding];
	[commandBuffer presentDrawable:drawable];
	[commandBuffer commit];
}
@end

namespace clv::plt{
    MacWindow::MacWindow(const WindowProps& props){
		initialiseWindow(props);
    }

	MacWindow::~MacWindow(){
		[windowProxy release];
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
				}
				[NSApp sendEvent: event];
			} while (event != nil);
		}
	}
	
	void MacWindow::initialiseWindow(const WindowProps& props){		
		windowProperties.width = props.width;
		windowProperties.height = props.height;
		windowProperties.title = props.title;
		
		NSString* nameString = [NSString stringWithCString:props.title.c_str() encoding:[NSString defaultCStringEncoding]];
		
		windowProxy = [[MacWindowProxy alloc] initWithWindowData:props.width
															 height:props.height
															   name:nameString];
		windowProxy.cloveWindow = this;
		
		surface = gfx::RenderCommand::createSurface(nullptr);
		gfx::RenderCommand::makeSurfaceCurrent(*surface);
	}
}
