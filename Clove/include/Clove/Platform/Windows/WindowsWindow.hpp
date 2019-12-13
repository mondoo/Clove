#pragma once

#include "Clove/Core/Platform/Window.hpp"

#include "Clove/Platform/Windows/WindowsException.hpp"

#define CLV_WINDOWS_QUIT 25397841 //Note: this number is completely random

namespace clv::plt{
	struct WindowsData{
		HWND handle;
		uint32 width;
		uint32 height;
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
		WindowsWindow(const WindowProps& props);
		WindowsWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size);

		WindowsWindow(const WindowsWindow& other) = delete;
		WindowsWindow(WindowsWindow&& other) noexcept = delete;

		WindowsWindow& operator=(const WindowsWindow& other) = delete;
		WindowsWindow& operator=(WindowsWindow&& other) noexcept = delete;

		virtual ~WindowsWindow();

		virtual void* getNativeWindow() const override;
		
	protected:
		virtual void processInput() override;

	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}