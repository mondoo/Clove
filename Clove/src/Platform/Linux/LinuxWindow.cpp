#include "Clove/Platform/Linux/LinuxWindow.hpp"

#include "Clove/Graphics/Core/Graphics.hpp"
#include "Clove/Graphics/Core/GraphicsFactory.hpp"
#include "Clove/Graphics/Core/Surface.hpp"

namespace clv::plt{
	LinuxWindow::LinuxWindow(const WindowDescriptor& descriptor){
        CLV_LOG_TRACE("Creating window: {0} ({1}, {2})", descriptor.title, descriptor.width, descriptor.height);

        display = XOpenDisplay(nullptr); //makes the connection to the client, where to display the window

        if(!display){
            //TODO: Exception
            CLV_LOG_ERROR("Could not open display");
            return;
        }

        screen = DefaultScreenOfDisplay(display); //Get the screen of the display
        screenID = DefaultScreen(display);

		graphicsFactory = gfx::initialise(descriptor.api);

		//Create the context first to get the visual info
        data = { display, &window, &visual };
        surface = graphicsFactory->createSurface(&data);

        if(screenID != visual->screen){
            //TODO: Exception
            CLV_LOG_CRITICAL("Screen ID does not match visual->screen");
            return;
        }

        windowAttribs = {};
        windowAttribs.border_pixel      = BlackPixel(display, screenID);
        windowAttribs.background_pixel  = WhitePixel(display, screenID);
        windowAttribs.override_redirect = true;
        windowAttribs.colormap          = XCreateColormap(display, RootWindow(display, screenID), visual->visual, AllocNone);
        windowAttribs.event_mask        = ExposureMask;

        window = XCreateWindow(display, RootWindow(display, screenID),
                               0, 0, descriptor.width, descriptor.height,
                               0, visual->depth, InputOutput, visual->visual,
                               CWBackPixel | CWColormap | CWBorderPixel | CWEventMask,
                               &windowAttribs);

        //Now that we have a window, we can make the context current
        surface->makeCurrent();

        //Remap the delete window message so we can gracefully close the application
        atomWmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", false);
        XSetWMProtocols(display, window, &atomWmDeleteWindow, 1);

        XSync(display, false); //Passing true here flushes the event queue

        const long keyboardMask = KeyPressMask | KeyReleaseMask | KeymapStateMask;
        const long mouseMask = PointerMotionMask | ButtonPressMask | ButtonReleaseMask | EnterWindowMask | LeaveWindowMask;

        XSelectInput(display, window, keyboardMask | mouseMask | StructureNotifyMask);

        XStoreName(display, window, descriptor.title.c_str());

        XClearWindow(display, window);
        XMapRaised(display, window);

        CLV_LOG_DEBUG("Window created");
	}

	LinuxWindow::LinuxWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size, const gfx::API api){
        CLV_LOG_TRACE("Creating child window: ({1}, {2})", size.x, size.y);

        const ::Window* nativeParentWindow = reinterpret_cast<::Window*>(parentWindow.getNativeWindow());

        display = XOpenDisplay(nullptr); //makes the connection to the client, where to display the window

        if(!display){
            //TODO: Exception
            CLV_LOG_ERROR("Could not open display");
            return;
        }

        screen = DefaultScreenOfDisplay(display); //Get the screen of the display
        screenID = DefaultScreen(display);

		graphicsFactory = gfx::initialise(api);

		//Create the context first to get the visual info
        data = { display, &window, &visual };
        surface = graphicsFactory->createSurface(&data);

        if(screenID != visual->screen){
            //TODO: Exception
            CLV_LOG_CRITICAL("Screen ID does not match visual->screen");
            return;
        }

        windowAttribs = {};
        windowAttribs.border_pixel      = BlackPixel(display, screenID);
        windowAttribs.background_pixel  = WhitePixel(display, screenID);
        windowAttribs.override_redirect = true;
        windowAttribs.colormap          = XCreateColormap(display, RootWindow(display, screenID), visual->visual, AllocNone);
        windowAttribs.event_mask        = ExposureMask;

        window = XCreateWindow(display, *nativeParentWindow,
                               position.x, position.y, size.x, size.y,
                               0, visual->depth, InputOutput, visual->visual,
                               CWBackPixel | CWColormap | CWBorderPixel | CWEventMask,
                               &windowAttribs);

        //Now that we have a window, we can make the context current
        surface->makeCurrent();

        //Remap the delete window message so we can gracefully close the application
        atomWmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", false);
        XSetWMProtocols(display, window, &atomWmDeleteWindow, 1);

        XSync(display, false); //Passing true here flushes the event queue

        const long keyboardMask = KeyPressMask | KeyReleaseMask | KeymapStateMask;
        const long mouseMask = PointerMotionMask | ButtonPressMask | ButtonReleaseMask | EnterWindowMask | LeaveWindowMask;

        XSelectInput(display, window, keyboardMask | mouseMask | StructureNotifyMask);

        XClearWindow(display, window);
        XMapRaised(display, window);

        CLV_LOG_DEBUG("Window created");
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
		return const_cast<::Window*>(&window);
	}

    mth::vec2i LinuxWindow::getPosition() const{
        ::Window rootWindow;
        int32_t posX;
        int32_t posY;
        uint32_t width;
        uint32_t height;
        uint32_t borderWidth;
        uint32_t depth;

        if(XGetGeometry(display, window, &rootWindow, &posX, &posY, &width, &height, &borderWidth, &depth) != 0){
            return { posX, posY };
        }else{
            CLV_ASSERT(false, "Could not get window geometry");
            return { 0, 0 };
        }
    }

    mth::vec2i LinuxWindow::getSize() const{
        ::Window rootWindow;
        int32_t posX;
        int32_t posY;
        uint32_t width;
        uint32_t height;
        uint32_t borderWidth;
        uint32_t depth;

        if(XGetGeometry(display, window, &rootWindow, &posX, &posY, &width, &height, &borderWidth, &depth) != 0){
            return { width, height };
        }else{
            CLV_ASSERT(false, "Could not get window geometry");
            return { 0, 0 };
        }
    }

    void LinuxWindow::moveWindow(const mth::vec2i &position) {
        XMoveWindow(display, window, position.x, position.y);
    }

    void LinuxWindow::resizeWindow(const mth::vec2i &size) {
        XResizeWindow(display, window, size.x, size.y);
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
						if(static_cast<uint32_t>(xce.width) != prevConfigureNotifySize.x || static_cast<uint32_t>(xce.height) != prevConfigureNotifySize.y){
							const mth::vec2i size{ xce.width, xce.height };
							prevConfigureNotifySize = size;
							if(surface){
								surface->resizeBuffers(size);
							}
							onWindowResize.broadcast(size);
						}
					}
					break;
			}
		}
	}
}