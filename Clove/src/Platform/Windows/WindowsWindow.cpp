#include "clvpch.hpp"
#include "WindowsWindow.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Graphics/Context.hpp"
#include "Clove/Events/ApplicationEvent.hpp"
#include "Clove/Events/MouseEvent.hpp"
#include "Clove/Events/KeyEvent.hpp"
#include "Clove/Application.hpp"

namespace clv{
	WindowsWindow::WindowsWindow(const WindowProps& props){
		initialiseWindow(props, gfx::API::DirectX11);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props, gfx::API api){
		initialiseWindow(props, api);
	}

	WindowsWindow::~WindowsWindow(){
		//Reset context first, before the window is destroyed
		context.reset();

		UnregisterClass(className, instance);
		DestroyWindow(windowsHandle);
	}

	void* WindowsWindow::getNativeWindow() const{
		return windowsHandle;
	}

	void WindowsWindow::setVSync(bool enabled){
		vSync = enabled;

		//move to renderer?
		/*typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
		PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;

		const char *extensions = (char*)glGetString(GL_EXTENSIONS);

		if(strstr(extensions, "WGL_EXT_swap_control") == 0){
			CLV_ERROR("Could not find the WGL_EXT_swap_control extension");
			return;
		} else{
			wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");

			if(wglSwapIntervalEXT){
				wglSwapIntervalEXT(enabled ? 1 : 0);
			}
		}*/
	}

	void WindowsWindow::processInput(){
		MSG msg;
		while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){
			if(msg.wParam == CLV_WINDOWS_QUIT){
				WindowCloseEvent event;
				eventCallback(event);
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	bool WindowsWindow::isVSync() const{
		return vSync;
	}

	LRESULT CALLBACK WindowsWindow::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
		if(msg == WM_NCCREATE){
			//Extract the ptr to our window class
			const CREATESTRUCTW* const create = reinterpret_cast<CREATESTRUCTW*>(lParam);
			WindowsWindow* const window = static_cast<WindowsWindow*>(create->lpCreateParams);
			//Store our windows class into the windows api
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
			//Switch over to the normal procedure handler
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowsWindow::HandleMsgThunk));

			return window->HandleMsg(hWnd, msg, wParam, lParam);
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK WindowsWindow::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
		//Forward message to our windows instance
		WindowsWindow* const window = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return window->HandleMsg(hWnd, msg, wParam, lParam);
	}

	LRESULT WindowsWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
		if(ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)){
			return true;
		}
		
		const POINTS pt = MAKEPOINTS(lParam);

		switch(msg){
			case WM_CLOSE:
				PostQuitMessage(CLV_WINDOWS_QUIT);
				return 0;

			case WM_KILLFOCUS:
				keyboard.clearState();
				break;

				//Keyboard
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				if(!(lParam & BIT(30)) || keyboard.isAutoRepeatEnabled()){
					keyboard.onKeyPressed(static_cast<Key>(wParam));
				}
				break;

			case WM_KEYUP:
			case WM_SYSKEYUP:
				keyboard.onKeyReleased(static_cast<Key>(wParam));
				break;

			case WM_CHAR:
				keyboard.onChar(static_cast<char>(wParam));
				break;

				//Mouse
			case WM_MOUSEMOVE:
				if(pt.x >= 0 && pt.x < windowProperties.width && pt.y >= 0 && pt.y < windowProperties.height){
					mouse.onMouseMove(pt.x, pt.y);
					if(!mouse.isInWindow()){
						mouse.onMouseEnter();
						SetCapture(hWnd);
					}
				} else{
					if(mouse.isButtonPressed(MouseButton::Left) || mouse.isButtonPressed(MouseButton::Right)){
						mouse.onMouseMove(pt.x, pt.y);
					} else{
						mouse.onMouseLeave();
						ReleaseCapture();
					}
				}
				break;

			case WM_LBUTTONDOWN:
				mouse.onButtonPressed(MouseButton::Left, pt.x, pt.y);
				break;

			case WM_LBUTTONUP:
				mouse.onButtonReleased(MouseButton::Left, pt.x, pt.y);
				break;

			case WM_RBUTTONDOWN:
				mouse.onButtonPressed(MouseButton::Right, pt.x, pt.y);
				break;

			case WM_RBUTTONUP:
				mouse.onButtonReleased(MouseButton::Right, pt.x, pt.y);
				break;

			case WM_XBUTTONDOWN:
				mouse.onButtonPressed(static_cast<MouseButton>(GET_KEYSTATE_WPARAM(wParam)), pt.x, pt.y);
				break;

			case WM_XBUTTONUP:
				mouse.onButtonReleased(static_cast<MouseButton>(GET_KEYSTATE_WPARAM(wParam)), pt.x, pt.y);
				break;

			case WM_MOUSEWHEEL:
				mouse.onWheelDelta(GET_WHEEL_DELTA_WPARAM(wParam), pt.x, pt.y);
				break;

		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	void WindowsWindow::initialiseWindow(const WindowProps& props, gfx::API api){
		windowProperties.title = props.title;
		windowProperties.width = props.width;
		windowProperties.height = props.height;
		
		instance = GetModuleHandle(nullptr);

		CLV_LOG_TRACE("Creating window: {0} ({1}, {2})", windowProperties.title, windowProperties.width, windowProperties.height);

		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = HandleMsgSetup;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = instance;
		wc.hIcon = nullptr;
		wc.hCursor = nullptr;
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = className;

		RegisterClassEx(&wc);

		CLV_LOG_DEBUG("Windows class registered");

		const std::wstring wideTitle(windowProperties.title.begin(), windowProperties.title.end());

		DWORD windowStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

		//Create a rect so we can adjust the resolution to be the client region not the entire window size
		RECT wr;
		wr.left = 100; //What is with these 100s???
		wr.right = windowProperties.width + wr.left;
		wr.top = 100;
		wr.bottom = windowProperties.height + wr.top;
		if(!AdjustWindowRect(&wr, windowStyle, FALSE)){
			throw CLV_WINDOWS_LAST_EXCEPTION;
		}

		windowsHandle = CreateWindow(
			wc.lpszClassName,
			wideTitle.c_str(),
			windowStyle,
			CW_USEDEFAULT, CW_USEDEFAULT,
			wr.right - wr.left, wr.bottom - wr.top,
			nullptr,
			nullptr,
			instance,
			this
		);

		if(!windowsHandle){
			throw CLV_WINDOWS_LAST_EXCEPTION;
		}

		CLV_LOG_INFO("Window created");

		ShowWindow(windowsHandle, SW_SHOW);

		data = { windowsHandle, windowProperties.width, windowProperties.height };

		context = gfx::Context::createContext(&data, api);
		context->makeCurrent();
		renderer = gfx::Renderer::createRenderer(*context, api);

		setVSync(true);
	}

	Window* Window::create(const WindowProps& props){
		return new WindowsWindow(props);
	}

	Window* Window::create(const WindowProps& props, gfx::API api){
		return new WindowsWindow(props, api);
	}
}
