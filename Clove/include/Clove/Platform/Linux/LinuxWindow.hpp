#pragma once

#include "Clove/Platform/Window.hpp"

#include "Clove/Platform/Linux/CloveLinux.hpp"

namespace clv::gfx {
	class GraphicsFactory;
}

namespace clv::plt {
	class LinuxWindow : public Window {
		//VARIABLES
	private:
		bool open = false;

		Display* display = nullptr;
		Screen* screen = nullptr;
		int32_t screenID;
		::Window window;

		XEvent xevent;

		XVisualInfo* visual = nullptr; //TODO: unique_ptr with custom deleter
		XSetWindowAttributes windowAttribs;

		Atom atomWmDeleteWindow;

		mth::vec2i prevConfigureNotifySize = {};

		//FUNCTIONS
	public:
		LinuxWindow() = delete;
		LinuxWindow(const WindowDescriptor& descriptor);
		LinuxWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size, const gfx::API api);

		LinuxWindow(const LinuxWindow& other) = delete;
		LinuxWindow(LinuxWindow&& other) noexcept = delete;

		LinuxWindow& operator=(const LinuxWindow& other) = delete;
		LinuxWindow& operator=(LinuxWindow&& other) noexcept = delete;

		~LinuxWindow();

		void* getNativeWindow() const override;

		mth::vec2i getPosition() const override;
		mth::vec2i getSize() const override;

		void moveWindow(const mth::vec2i& position) override;
		void resizeWindow(const mth::vec2i& size) override;

		bool isOpen() const override;

		void close() override;

	protected:
		void processInput() override;
	};
}