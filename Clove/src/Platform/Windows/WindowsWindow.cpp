#include "Clove/Platform/Windows/WindowsWindow.hpp"

#include "Clove/Core/Graphics/RenderCommand.hpp"
#include "Clove/Core/Graphics/Surface.hpp"

namespace clv::plt{
	WindowsWindow::WindowsWindow(const WindowProps& props){
		instance = GetModuleHandle(nullptr);

		CLV_LOG_TRACE("Creating window: {0} ({1}, {2})", props.title, props.width, props.height);

		WNDCLASSEX wc{};
		wc.cbSize			= sizeof(wc);
		wc.style			= CS_OWNDC;
		wc.lpfnWndProc		= HandleMsgSetup;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= 0;
		wc.hInstance		= instance;
		wc.hIcon			= nullptr;
		wc.hCursor			= nullptr;
		wc.hbrBackground	= nullptr;
		wc.lpszMenuName		= nullptr;
		wc.lpszClassName	= className;

		RegisterClassEx(&wc);

		CLV_LOG_TRACE("Windows class registered");

		const std::string wideTitle(props.title.begin(), props.title.end());

		const DWORD windowStyle = WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX | WS_SYSMENU | WS_VISIBLE;

		windowsHandle = CreateWindow(
			wc.lpszClassName,
			wideTitle.c_str(),
			windowStyle,
			CW_USEDEFAULT, CW_USEDEFAULT,
			props.width, props.height,
			nullptr,
			nullptr,
			instance,
			this
		);

		if(!windowsHandle){
			throw CLV_WINDOWS_LAST_EXCEPTION;
		}

		CLV_LOG_DEBUG("Window created");

		data = { windowsHandle, props.width, props.height };

		surface = gfx::RenderCommand::createSurface(&data);
		gfx::RenderCommand::makeSurfaceCurrent(surface);
	}

	WindowsWindow::WindowsWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size){
		CLV_LOG_TRACE("Creating child window: ({1}, {2})", size.x, size.y);

		WNDCLASSEX wc{};
		wc.cbSize			= sizeof(wc);
		wc.style			= CS_OWNDC;
		wc.lpfnWndProc		= HandleMsgSetup;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= 0;
		wc.hInstance		= instance;
		wc.hIcon			= nullptr;
		wc.hCursor			= nullptr;
		wc.hbrBackground	= nullptr;
		wc.lpszMenuName		= nullptr;
		wc.lpszClassName	= className;

		RegisterClassEx(&wc);

		CLV_LOG_TRACE("Windows class registered");

		const DWORD windowStyle = WS_CHILD | WS_VISIBLE;

		windowsHandle = CreateWindow(
			wc.lpszClassName,
			"",
			windowStyle,
			position.x, position.y,
			size.x, size.y,
			reinterpret_cast<HWND>(parentWindow.getNativeWindow()),
			nullptr,
			instance,
			this
		);

		if(!windowsHandle){
			throw CLV_WINDOWS_LAST_EXCEPTION;
		}

		CLV_LOG_DEBUG("Window created");

		data = { windowsHandle, size.x, size.y };

		surface = gfx::RenderCommand::createSurface(&data);
		gfx::RenderCommand::makeSurfaceCurrent(surface);
	}

	WindowsWindow::~WindowsWindow(){
		//Reset context first, before the window is destroyed
		surface.reset();

		UnregisterClass(className, instance);
		DestroyWindow(windowsHandle);
	}

	void* WindowsWindow::getNativeWindow() const{
		return windowsHandle;
	}

	mth::vec2i WindowsWindow::getPosition() const{
		RECT windowRect;
		GetWindowRect(windowsHandle, &windowRect);
		MapWindowPoints(windowsHandle, GetParent(windowsHandle), (LPPOINT)&windowRect, 2);

		return { windowRect.left, windowRect.top };
	}

	mth::vec2i WindowsWindow::getSize() const{
		RECT windowRect;
		GetWindowRect(windowsHandle, &windowRect);
		MapWindowPoints(windowsHandle, GetParent(windowsHandle), (LPPOINT)&windowRect, 2);

		return { windowRect.right - windowRect.left, windowRect.bottom - windowRect.top };
	}

	void WindowsWindow::moveWindow(const mth::vec2i& position){
		const mth::vec2i size = getSize();
		const BOOL resized = MoveWindow(windowsHandle, position.x, position.y, size.x, size.y, FALSE);
		if(!resized){
			throw CLV_WINDOWS_LAST_EXCEPTION;
		}
	}

	void WindowsWindow::resizeWindow(const mth::vec2i& size){
		const mth::vec2i position = getPosition();
		const BOOL resized = MoveWindow(windowsHandle, position.x, position.y, size.x, size.y, FALSE);
		if(!resized){
			throw CLV_WINDOWS_LAST_EXCEPTION;
		}
	}
	
	void WindowsWindow::processInput(){
		MSG msg;
		while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){
			if(msg.wParam == CLV_WINDOWS_QUIT){
				onWindowCloseDelegate.broadcast();
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
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
				if(pt.x >= 0 && pt.x < getSize().x && pt.y >= 0 && pt.y < getSize().y){
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

				//Window
			case WM_SIZE:
				{
					const mth::vec2ui size = { pt.x, pt.y };
					if(surface){ //Can be called before the surface is initialised
						surface->resizeBuffers(size);
						gfx::RenderCommand::makeSurfaceCurrent(surface);
					}
					onWindowResize.broadcast(size);
				}
				break;

		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}
