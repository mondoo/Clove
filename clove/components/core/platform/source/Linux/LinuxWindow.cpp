#include "Clove/Platform/Linux/LinuxWindow.hpp"

#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    LinuxWindow::LinuxWindow(WindowDescriptor const &descriptor)
        : Window(keyboardDispatcher, mouseDispatcher) {
        CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Trace, "Creating window: {0} ({1}, {2})", descriptor.title, descriptor.width, descriptor.height);

        display = XOpenDisplay(nullptr);//makes the connection to the client, where to display the window

        if(display == nullptr) {
            //TODO: Exception
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Could not open display");
            return;
        }

        screen   = DefaultScreenOfDisplay(display);//Get the screen of the display
        screenID = DefaultScreen(display);

        windowAttribs                   = {};
        windowAttribs.border_pixel      = BlackPixel(display, screenID);
        windowAttribs.background_pixel  = WhitePixel(display, screenID);
        windowAttribs.override_redirect = 1;
        windowAttribs.colormap          = XCreateColormap(display, RootWindow(display, screenID), screen->root_visual, AllocNone);
        windowAttribs.event_mask        = ExposureMask;

        window = XCreateWindow(display, RootWindow(display, screenID), 0, 0, descriptor.width, descriptor.height, 0, screen->depths[0].depth, InputOutput, screen->root_visual, CWBackPixel | CWColormap | CWBorderPixel | CWEventMask, &windowAttribs);

        //Remap the delete window message so we can gracefully close the application
        atomWmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", 0);
        XSetWMProtocols(display, window, &atomWmDeleteWindow, 1);

        XSync(display, 0);//Passing true here flushes the event queue

        long const keyboardMask{ KeyPressMask | KeyReleaseMask | KeymapStateMask };
        long const mouseMask{ PointerMotionMask | ButtonPressMask | ButtonReleaseMask | EnterWindowMask | LeaveWindowMask };

        XSelectInput(display, window, keyboardMask | mouseMask | StructureNotifyMask);

        XStoreName(display, window, descriptor.title.c_str());

        XClearWindow(display, window);
        XMapRaised(display, window);

        open = true;

        CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Trace, "Window created");
    }

    LinuxWindow::~LinuxWindow() {
        XFreeColormap(display, windowAttribs.colormap);
        XDestroyWindow(display, window);
        XCloseDisplay(display);
    }

    std::any LinuxWindow::getNativeWindow() const {
        return std::make_pair(display, window);
    }

    vec2i LinuxWindow::getPosition() const {
        ::Window rootWindow{ 0 };
        int32_t posX{ 0 };
        int32_t posY{ 0 };
        uint32_t width{ 0 };
        uint32_t height{ 0 };
        uint32_t borderWidth{ 0 };
        uint32_t depth{ 0 };

        if(XGetGeometry(display, window, &rootWindow, &posX, &posY, &width, &height, &borderWidth, &depth) != 0) {
            return { posX, posY };
        } else {
            CLOVE_ASSERT(false, "Could not get window geometry");
            return { 0, 0 };
        }
    }

    vec2i LinuxWindow::getSize() const {
        ::Window rootWindow{ 0 };
        int32_t posX{ 0 };
        int32_t posY{ 0 };
        uint32_t width{ 0 };
        uint32_t height{ 0 };
        uint32_t borderWidth{ 0 };
        uint32_t depth{ 0 };

        if(XGetGeometry(display, window, &rootWindow, &posX, &posY, &width, &height, &borderWidth, &depth) != 0) {
            return { width, height };
        } else {
            CLOVE_ASSERT(false, "Could not get window geometry");
            return { 0, 0 };
        }
    }

    void LinuxWindow::moveWindow(vec2i const &position) {
        XMoveWindow(display, window, position.x, position.y);
    }

    void LinuxWindow::resizeWindow(vec2i const &size) {
        XResizeWindow(display, window, size.x, size.y);
    }

    bool LinuxWindow::isOpen() const {
        return open;
    }

    void LinuxWindow::close() {
        if(onWindowCloseDelegate.isBound()) {
            onWindowCloseDelegate.broadcast();
        }
        open = false;
        XCloseDisplay(display);
    }

    void LinuxWindow::processInput() {
        if(XPending(display) > 0) {
            KeySym xkeysym{ 0 };

            XNextEvent(display, &xevent);
            switch(xevent.type) {
                case ClientMessage:
                    if(xevent.xclient.data.l[0] == atomWmDeleteWindow) {
                        close();
                    }
                    break;
                case DestroyNotify:
                    if(onWindowCloseDelegate.isBound()) {
                        onWindowCloseDelegate.broadcast();
                    }
                    open = false;
                    break;

                case FocusOut:
                    keyboardDispatcher.clearState();
                    break;

                case KeymapNotify:
                    //Refresh key mappings if the user rebinds keys
                    XRefreshKeyboardMapping(&xevent.xmapping);
                    break;

                case KeyPress:
                    xkeysym = XLookupKeysym(&xevent.xkey, 0);
                    keyboardDispatcher.onKeyPressed(static_cast<Key>(xkeysym));
                    break;

                case KeyRelease: {
                    bool isRepeat = false;

                    if(XEventsQueued(display, QueuedAlready) != 0) {
                        XEvent nextEvent;
                        XPeekEvent(display, &nextEvent);

                        isRepeat = nextEvent.type == KeyPress && nextEvent.xkey.keycode == xevent.xkey.keycode;
                        if(isRepeat) {
                            //Consume the next KeyPress event (as that is auto repeat)
                            XNextEvent(display, &xevent);
                        }
                    }

                    if(!isRepeat || keyboard.isAutoRepeatEnabled()) {
                        xkeysym = XLookupKeysym(&xevent.xkey, 0);
                        keyboardDispatcher.onKeyReleased(static_cast<Key>(xkeysym));
                    }
                } break;

                    //TODO: Char (I don't think Xlib has a 'typed' event)

                case EnterNotify:
                    mouseDispatcher.onMouseEnter();
                    break;

                case LeaveNotify:
                    mouseDispatcher.onMouseLeave();
                    break;

                case ButtonPress:
                    if(xevent.xbutton.button == Button4) {
                        mouseDispatcher.onWheelDelta(CLV_WHEEL_DELTA, xevent.xbutton.x, xevent.xbutton.y);
                    } else if(xevent.xbutton.button == Button5) {
                        mouseDispatcher.onWheelDelta(-CLV_WHEEL_DELTA, xevent.xbutton.x, xevent.xbutton.y);
                    } else {
                        mouseDispatcher.onButtonPressed(static_cast<MouseButton>(xevent.xbutton.button), xevent.xbutton.x, xevent.xbutton.y);
                    }
                    break;

                case ButtonRelease:
                    mouseDispatcher.onButtonReleased(static_cast<MouseButton>(xevent.xbutton.button), xevent.xbutton.x, xevent.xbutton.y);
                    break;

                //Window
                case ConfigureNotify: {
                    XConfigureEvent xce = xevent.xconfigure;
                    if(static_cast<uint32_t>(xce.width) != prevConfigureNotifySize.x || static_cast<uint32_t>(xce.height) != prevConfigureNotifySize.y) {
                        const vec2i size{ xce.width, xce.height };
                        prevConfigureNotifySize = size;
                        onWindowResize.broadcast(size);
                    }
                } break;
            }
        }
    }
}