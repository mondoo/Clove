#pragma once

#include "Clove/Platform/Core/Window.hpp"

#include "Clove/Platform/Windows/WindowsException.hpp"
#include "Clove/Platform/Windows/CloveWindows.hpp"

namespace clv::plt{
	struct WindowsData{
		HWND handle;
		int32_t width;
		int32_t height;
	};

	class WindowsWindow : public Window{
		//VARIABLES
	private:
		static constexpr LPCSTR className = "Clove";

		HINSTANCE instance;
		HWND windowsHandle;

		WindowsData data;

		//FUNCTIONS
	public:
		WindowsWindow() = delete;
		WindowsWindow(const WindowDescriptor& descriptor);
		WindowsWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size, const gfx::API api);

		WindowsWindow(const WindowsWindow& other) = delete;
		WindowsWindow(WindowsWindow&& other) noexcept = delete;

		WindowsWindow& operator=(const WindowsWindow& other) = delete;
		WindowsWindow& operator=(WindowsWindow&& other) noexcept = delete;

		virtual ~WindowsWindow();

		virtual void* getNativeWindow() const override;

		virtual mth::vec2i getPosition() const override;
		virtual mth::vec2i getSize() const override;

		virtual void moveWindow(const mth::vec2i& position) override;
		virtual void resizeWindow(const mth::vec2i& size) override;
		
	protected:
		virtual void processInput() override;

	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}