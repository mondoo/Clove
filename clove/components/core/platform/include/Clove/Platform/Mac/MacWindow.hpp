#pragma once

#include "Clove/Platform/Window.hpp"

#include "Clove/Platform/Mac/CloveMac.hpp"

namespace garlic::clove {
    class Window;
	class MacWindow;
}

@interface MacWindowProxy : NSWindow <NSWindowDelegate>

@property garlic::clove::MacWindow *cloveWindow;

@end

namespace garlic::clove {
    class MacWindow : public Window{
		//VARIABLES
	private:
		bool open = false;

		MacWindowProxy* windowProxy;

        Keyboard::Dispatcher keyboardDispatcher{};
        Mouse::Dispatcher mouseDispatcher{};

        //FUNCTIONS
	public:
		MacWindow() = delete;
		MacWindow(const WindowDescriptor& descriptor);

		MacWindow(const MacWindow& other) = delete;
		MacWindow(MacWindow&& other) noexcept = delete;

		MacWindow& operator=(const MacWindow& other) = delete;
		MacWindow& operator=(MacWindow&& other) noexcept = delete;

		~MacWindow();
		
		void processInput() override;

		std::any getNativeWindow() const override;

        vec2i getPosition(bool clientArea) const override;
        vec2i getSize() const override;

		void moveWindow(const vec2i& position) override;
		void resizeWindow(const vec2i& size) override;

		bool isOpen() const override;

		void close() override;
		
		void handleNsEvent(NSEvent* event);
	};
}
