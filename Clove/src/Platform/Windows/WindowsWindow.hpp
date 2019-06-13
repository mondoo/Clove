#pragma once

#include "Clove/Platform/Window.hpp"

#include "Platform/Windows/WindowsException.hpp"

#define CLV_WINDOWS_QUIT 25397841 //Note: this number is completely random

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace clv{
	struct WindowsData{
		HWND handle;
		unsigned int width;
		unsigned int height;
	};

	class WindowsWindow : public Window{
		//VARIABLES
	private:
		static constexpr LPWSTR className = L"Clove";

		HINSTANCE instance;
		HWND windowsHandle;

		WindowsData data;

		//FUNCTIONS
	public:
		WindowsWindow() = delete;
		WindowsWindow(const WindowsWindow& other) = delete;
		WindowsWindow(WindowsWindow&& other) noexcept = delete;
		WindowsWindow& operator=(const WindowsWindow& other) = delete;
		WindowsWindow& operator=(WindowsWindow&& other) noexcept = delete;
		virtual ~WindowsWindow();
		
		WindowsWindow(const WindowProps& props);
		WindowsWindow(const WindowProps& props, gfx::API api);

		virtual void* getNativeWindow() const override;

		virtual void setVSync(bool enabled) override;
		virtual bool isVSync() const override;

	protected:
		virtual void processInput() override;

	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		void initialiseWindow(const WindowProps& props, gfx::API api);
	};
}