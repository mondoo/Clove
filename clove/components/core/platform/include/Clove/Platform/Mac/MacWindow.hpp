#pragma once

#include "Clove/Platform/Window.hpp"

#include "Clove/Platform/Mac/CloveMac.hpp"

namespace garlic::clove {
    class Window;
	class MacWindow;
}

@interface MacWindowProxy : NSObject <NSWindowDelegate>

@property(readonly) NSWindow* window;
@property garlic::clove::MacWindow *cloveWindow;

- (instancetype)initWithWindowData:(NSView*)view width:(unsigned int)width height:(unsigned int)height name: (NSString*)name;

@end

namespace garlic::clove{
	class GraphicsFactory;
}

namespace garlic::clove {
    class MacWindow : public Window{
		//VARIABLES
	private:
		bool open = false;

		MacWindowProxy* windowProxy;

		//FUNCTIONS
	public:
		MacWindow() = delete;
		MacWindow(const WindowDescriptor& descriptor);

		MacWindow(const MacWindow& other) = delete;
		MacWindow(MacWindow&& other) noexcept = delete;

		MacWindow& operator=(const MacWindow& other) = delete;
		MacWindow& operator=(MacWindow&& other) noexcept = delete;

		~MacWindow();

		std::any getNativeWindow() const override;
		
		vec2i getPosition() const override;
		vec2i getSize() const override;

		void moveWindow(const vec2i& position) override;
		void resizeWindow(const vec2i& size) override;

		bool isOpen() const override;

		void close() override;

	protected:
		virtual void processInput() override;
	};
}
