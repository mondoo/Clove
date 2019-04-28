#pragma once

#include "Clove/Window.hpp"

#include "Platform/Windows/WindowsException.hpp"

struct GLFWwindow;

#define CLV_WINDOWS_EXCEPTION(hr) WindowsException(__LINE__, __FILE__, hr)
#define CLV_WINDOWS_LAST_EXCEPTION WindowsException(__LINE__, __FILE__, GetLastError())

#define CLV_WINDOWS_QUIT 25397841

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace clv{
	class WindowsWindow : public Window{
		//VARIABLES
	private:
		//TODO: Do we need this without glfw?
		//For GLFW 
		struct WindowData{
			std::string title = "";
			unsigned int width = 0;
			unsigned int height = 0;
			bool vSync = false;

			EventCallbackFn eventCallback;
		} data;

		static constexpr LPWSTR className = L"Clove";
		
		HINSTANCE instance;
		HWND windowsHandle;

		HDC windowsDeviceContext;
		HGLRC windowsResourceContext;

		//FUNCTIONS
	public:
		WindowsWindow() = delete;
		WindowsWindow(const WindowsWindow& other) = delete;
		WindowsWindow(WindowsWindow&& other) noexcept = delete;

		WindowsWindow(const WindowProps& props);
		
		virtual ~WindowsWindow();

		virtual void beginFrame() override;

		virtual inline void setEventCallbackFunction(const EventCallbackFn& callback) override;
		
		virtual void* getNativeWindow() const override;

		virtual inline unsigned int getWidth() const override;
		virtual inline unsigned int getHeight() const override;

		virtual void setVSync(bool enabled) override;
		virtual bool isVSync() const override;

		WindowsWindow& operator=(const WindowsWindow& other) = delete;
		WindowsWindow& operator=(WindowsWindow&& other) noexcept = delete;

	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}

#include "WindowsWindow.inl"