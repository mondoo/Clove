#pragma once

#include "Clove/Platform/Window.hpp"

#include "Clove/Platform/Windows/CloveWindows.hpp"
#include "Clove/Platform/Windows/WindowsException.hpp"

namespace clv::plt {
	class WindowsWindow : public Window {
		//VARIABLES
	private:
		bool open = false;

		static constexpr LPCSTR className = "Clove";

		HINSTANCE instance;
		HWND windowsHandle;

		//FUNCTIONS
	public:
		WindowsWindow() = delete;
		WindowsWindow(const WindowDescriptor& descriptor);
		WindowsWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size, const gfx::API api);

		WindowsWindow(const WindowsWindow& other) = delete;
		WindowsWindow(WindowsWindow&& other) noexcept = delete;

		WindowsWindow& operator=(const WindowsWindow& other) = delete;
		WindowsWindow& operator=(WindowsWindow&& other) noexcept = delete;

		~WindowsWindow();

		void* getNativeWindow() const override;

		mth::vec2i getPosition() const override;
		mth::vec2i getSize() const override;

		void moveWindow(const mth::vec2i& position) override;
		void resizeWindow(const mth::vec2i& size) override;

		bool isOpen() const override;

	protected:
		void processInput() override;

	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}