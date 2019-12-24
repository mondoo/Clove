#pragma once

#include "Clove/Core/Platform/Window.hpp"

//Temp metal stuff
#import <MetalKit/MetalKit.h>

namespace clv::plt{
	class MacWindow;
}

@interface MacWindowProxy : NSObject <NSWindowDelegate /*, MTKViewDelegate*/> //Temp view delegate

@property(readonly) NSWindow* window;
@property(readonly) MTKView* view;

@property(readonly) id<MTLCommandQueue> commandQueue;
@property(readonly) id<MTLDevice> device;

@property clv::plt::MacWindow* cloveWindow;

- (instancetype)initWithWindowData:(MTKView*)view width: (unsigned int)width height: (unsigned int)height name: (NSString*)name;

@end

namespace clv::plt{
	struct MacData{
		mth::vec2ui size{};
	};
}

namespace clv::plt{
	class MacWindow : public Window{
		//VARIABLES
	private:
		MacWindowProxy* windowProxy;

		//FUNCTIONS
	public:
		MacWindow() = delete;
		MacWindow(const WindowProps& props);

		MacWindow(const MacWindow& other) = delete;
		MacWindow(MacWindow&& other) noexcept = delete;

		MacWindow& operator=(const MacWindow& other) = delete;
		MacWindow& operator=(MacWindow&& other) noexcept = delete;

		virtual ~MacWindow();

		virtual void* getNativeWindow() const override;

	protected:
		virtual void processInput() override;

	private:
		void initialiseWindow(const WindowProps& props);
	};
}
