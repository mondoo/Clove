#include "clvpch.hpp"
#include "WindowsWindow.hpp"

#include "Clove/Events/ApplicationEvent.hpp"
#include "Clove/Events/MouseEvent.hpp"
#include "Clove/Events/KeyEvent.hpp"
#include "Clove/Rendering/Renderer.hpp"

#include "Clove/Rendering/API/GLHelpers.hpp"
#include <glad/glad.h>

namespace clv{
#if CLV_PLATFORM_WINDOWS
	Window* Window::create(const WindowProps& props){
		return new WindowsWindow(props);
	}
#endif

	WindowsWindow::WindowsWindow(const WindowProps& props)
		: instance(GetModuleHandle(nullptr)){

		data.title = props.title;
		data.width = props.width;
		data.height = props.height;

		CLV_TRACE("Creating window: {0} ({1}, {2})", data.title, data.width, data.height);

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

		CLV_TRACE("Windows class registered");

		const std::wstring wideTitle(data.title.begin(), data.title.end());

		DWORD windowStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

		//Create a rect so we can adjust the resolution to be the client region not the entire window size
		RECT wr;
		wr.left = 100;
		wr.right = data.width + wr.left;
		wr.top = 100;
		wr.bottom = data.height + wr.top;
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

		CLV_TRACE("Window created");

		hDC = GetDC(windowsHandle);

		PIXELFORMATDESCRIPTOR pfd = { 0 };
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cAlphaBits = 8;
		pfd.cDepthBits = 24;

		int pf;
		pf = ChoosePixelFormat(hDC, &pfd);
		if(pf == 0){
			throw CLV_WINDOWS_LAST_EXCEPTION;
		}

		if(SetPixelFormat(hDC, pf, &pfd) == FALSE){
			throw CLV_WINDOWS_LAST_EXCEPTION;
		}
		
		HGLRC hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);

		CLV_TRACE("Device context created");

		CLV_ASSERT(gladLoadGL(), "Failed to load OpenGL functions");

		ShowWindow(windowsHandle, SW_SHOW);

		CLV_INFO("Window created successfully!");
		CLV_INFO("GL version: {0}", glGetString(GL_VERSION));

		CLV_TRACE("Enabling Depth buffer");
		GLCall(glDepthFunc(GL_LESS));
		GLCall(glEnable(GL_DEPTH_TEST));

		CLV_TRACE("Blend set to: SRC_ALPHA | ONE_MINUS_SRC_ALPHA");
		//src is from the image - dest is what is already in the buffer
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));
		//I guess it's called blending because you blend the src with the destination

		setVSync(true);
	}

	WindowsWindow::~WindowsWindow(){
		UnregisterClass(className, instance);
		ReleaseDC(windowsHandle, hDC);
		DestroyWindow(windowsHandle);
	}

	void WindowsWindow::beginFrame(){
		SwapBuffers(hDC);

		MSG msg;
		while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){
			if(msg.wParam == CLV_WINDOWS_QUIT){
				WindowCloseEvent event;
				data.eventCallback(event);
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void WindowsWindow::setVSync(bool enabled){
		data.vSync = enabled;

		typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
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
		}
	}

	bool WindowsWindow::isVSync() const{
		return data.vSync;
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
				if(pt.x >= 0 && pt.x < data.width && pt.y >= 0 && pt.y < data.height){
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
}
