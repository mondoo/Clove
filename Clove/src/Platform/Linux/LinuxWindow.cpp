#include "Clove/Platform/Linux/LinuxWindow.hpp"

#include "Clove/Core/Graphics/Surface.hpp"
#include "Clove/Core/Graphics/RenderCommand.hpp"

#include <X11/Xlib.h>

namespace clv::plt{
	LinuxWindow::LinuxWindow(const WindowProps& props){
		initialiseWindow(props);
	}

	LinuxWindow::~LinuxWindow(){
		//Reset context first, before the display is closed
		surface.reset();

		XFree(visual);
		XFreeColormap(display, windowAttribs.colormap);
		XDestroyWindow(display, window);
		XCloseDisplay(display);
	}

	void* LinuxWindow::getNativeWindow() const{
		return display;
	}

	void LinuxWindow::processInput(){
		if(XPending(display) > 0){
			KeySym xkeysym = 0;

			XNextEvent(display, &xevent);
			switch(xevent.type){
				case ClientMessage:
					if(xevent.xclient.data.l[0] == atomWmDeleteWindow){
						onWindowCloseDelegate.broadcast();
					}
					break;
				case DestroyNotify:
					onWindowCloseDelegate.broadcast();
					break;

				case FocusOut:
					keyboard.clearState();
					break;

				case KeymapNotify:
					//Refresh key mappings if the user rebinds keys
					XRefreshKeyboardMapping(&xevent.xmapping);
					break;

				case KeyPress:
					xkeysym = XLookupKeysym(&xevent.xkey, 0);
					keyboard.onKeyPressed(static_cast<Key>(xkeysym));
					break;

				case KeyRelease:
					xkeysym = XLookupKeysym(&xevent.xkey, 0);
					keyboard.onKeyReleased(static_cast<Key>(xkeysym));
					break;

					//TODO: Char (I don't think Xlib has a 'typed' event)

				case EnterNotify:
					mouse.onMouseEnter();
					break;

				case LeaveNotify:
					mouse.onMouseLeave();
					break;

				case ButtonPress:
					if(xevent.xbutton.button == 4){
						mouse.onWheelDelta(CLV_WHEEL_DELTA, xevent.xbutton.x, xevent.xbutton.y);
					} else if(xevent.xbutton.button == 5){
						mouse.onWheelDelta(-CLV_WHEEL_DELTA, xevent.xbutton.x, xevent.xbutton.y);
					} else{
						mouse.onButtonPressed(static_cast<MouseButton>(xevent.xbutton.button), xevent.xbutton.x, xevent.xbutton.y);
					}
					break;

				case ButtonRelease:
					mouse.onButtonReleased(static_cast<MouseButton>(xevent.xbutton.button), xevent.xbutton.x, xevent.xbutton.y);
					break;

				//Window
				case ConfigureNotify:
					{
						XConfigureEvent xce = xevent.xconfigure;
						if(static_cast<uint32>(xce.width) != getWidth() || static_cast<uint32>(xce.height) != getHeight()){
							const mth::vec2i size{ xce.width, xce.height };
							if(surface){
								surface->resizeBuffers(size);
								gfx::RenderCommand::makeSurfaceCurrent(surface);
							}
							windowProperties.width = size.x;
							windowProperties.height = size.y;
							onWindowResize.broadcast(size);
						}
					}
					break;
			}
		}
	}

	void LinuxWindow::initialiseWindow(const WindowProps& props){
		windowProperties.title = props.title;
		windowProperties.width = props.width;
		windowProperties.height = props.height;

		display = XOpenDisplay(nullptr); //makes the connection to the client, where to display the window

		if(!display){
			//TODO: Exception
			CLV_LOG_ERROR("Could not open display");
			return;
		}

		screen = DefaultScreenOfDisplay(display); //Get the screen of the display
		screenID = DefaultScreen(display);

        //Create the context first to get the visual info
        data = { display, &window, &visual };
		surface = gfx::RenderCommand::createSurface(&data);

		if(screenID != visual->screen){
			//TODO: Exception
			CLV_LOG_CRITICAL("Screen ID does not match visual->screen");
			return;
		}

		windowAttribs = { 0 };
		windowAttribs.border_pixel = BlackPixel(display, screenID);
		windowAttribs.background_pixel = WhitePixel(display, screenID);
		windowAttribs.override_redirect = true;
		windowAttribs.colormap = XCreateColormap(display, RootWindow(display, screenID), visual->visual, AllocNone);
		windowAttribs.event_mask = ExposureMask;
        

		window = XCreateWindow(display, RootWindow(display, screenID),
							   0, 0, windowProperties.width, windowProperties.height,
							   0, visual->depth, InputOutput, visual->visual,
							   CWBackPixel | CWColormap | CWBorderPixel | CWEventMask,
							   &windowAttribs);

        //Now that we have a window, we can make the context current
		gfx::RenderCommand::makeSurfaceCurrent(surface);

		//Remap the delete window message so we can gracefully close the application
		atomWmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", false);
		XSetWMProtocols(display, window, &atomWmDeleteWindow, 1);

		XSync(display, false); //Passing true here flushes the event queue

		const long keyboardMask = KeyPressMask | KeyReleaseMask | KeymapStateMask;
		const long mouseMask = PointerMotionMask | ButtonPressMask | ButtonReleaseMask | EnterWindowMask | LeaveWindowMask;

		XSelectInput(display, window, keyboardMask | mouseMask | StructureNotifyMask);

		XStoreName(display, window, windowProperties.title.c_str());

		XClearWindow(display, window);
		XMapRaised(display, window);

		CLV_LOG_DEBUG("Window created");
	}
}