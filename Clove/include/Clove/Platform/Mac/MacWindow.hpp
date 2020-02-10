#pragma once

#include "Clove/Platform/Core/Window.hpp"

#include "Clove/Platform/Mac/CloveMac.hpp"

#import <MetalKit/MetalKit.h>

namespace clv::plt{
	class Window;
	class MacWindow;
}

@interface MacWindowProxy : NSObject <NSWindowDelegate>

@property(readonly) NSWindow* window;
@property clv::plt::MacWindow* cloveWindow;

- (instancetype)initWithWindowData:(MTKView*)view width:(unsigned int)width height:(unsigned int)height name: (NSString*)name;
- (instancetype)initWithParentWindow:(MTKView*)view parentWindow:(const clv::plt::Window&)parentWindow position:(const clv::mth::vec2i&)position size:(const clv::mth::vec2i&)size;

@end

namespace clv::gfx{
	class GraphicsFactory;
}

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
		MacWindow(const WindowDescriptor& descriptor);
		MacWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size, const gfx::API api);

		MacWindow(const MacWindow& other) = delete;
		MacWindow(MacWindow&& other) noexcept = delete;

		MacWindow& operator=(const MacWindow& other) = delete;
		MacWindow& operator=(MacWindow&& other) noexcept = delete;

		virtual ~MacWindow();

		virtual void* getNativeWindow() const override;
		
		virtual mth::vec2i getPosition() const override;
		virtual mth::vec2i getSize() const override;

		virtual void moveWindow(const mth::vec2i& position) override;
		virtual void resizeWindow(const mth::vec2i& size) override;

	protected:
		virtual void processInput() override;
	};
}
