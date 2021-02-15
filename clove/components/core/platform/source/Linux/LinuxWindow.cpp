#include "Clove/Platform/Linux/LinuxWindow.hpp"

#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>

namespace garlic::clove {
    LinuxWindow::LinuxWindow(Descriptor const &descriptor)
        : Window(keyboardDispatcher, mouseDispatcher) {
        CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Trace, "Creating window: {0} ({1}, {2})", descriptor.title, descriptor.width, descriptor.height);
        CLOVE_ASSERT(window == 0, "Window already exists! Currently only a single window on linux is supported");

        if(display == nullptr) {
            //Make the connection to the client, where to display the window
            display = XOpenDisplay(nullptr);
        }

        if(!display) {
            //TODO: Exception
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Could not open display");
            return;
        }

        screen   = DefaultScreenOfDisplay(display);//Get the screen of the display
        screenID = DefaultScreen(display);

        windowAttribs                   = {};
        windowAttribs.border_pixel      = BlackPixel(display, screenID);
        windowAttribs.background_pixel  = WhitePixel(display, screenID);
        windowAttribs.override_redirect = true;
        windowAttribs.colormap          = XDefaultColormap(display, screenID);
        windowAttribs.event_mask        = ExposureMask;

        window = XCreateWindow(display, RootWindow(display, screenID), 0, 0, descriptor.width, descriptor.height, 0, screen->depths[0].depth, InputOutput, screen->root_visual, CWBackPixel | CWColormap | CWBorderPixel | CWEventMask, &windowAttribs);

        //Remap the delete window message so we can gracefully close the application
        atomWmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", false);
        XSetWMProtocols(display, window, &atomWmDeleteWindow, 1);

        XSync(display, false);//Passing true here flushes the event queue

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
        XDestroyWindow(display, window);
        XCloseDisplay(display);
    }

    std::unique_ptr<Window> Window::create(Descriptor descriptor) {
        return std::make_unique<LinuxWindow>(std::move(descriptor));
    }

    std::any LinuxWindow::getNativeWindow() const {
        return std::make_pair(display, window);
    }

    vec2i LinuxWindow::getPosition(bool clientArea) const {
        int32_t x{ 0 };
        int32_t y{ 0 };
        ::Window child{};
        XTranslateCoordinates(display, window, XRootWindowOfScreen(screen), 0, 0, &x, &y, &child);

        if(clientArea) {
            return { x, y };
        } else {
            XWindowAttributes attribs{};
            XGetWindowAttributes(display, window, &attribs);
            return { x - attribs.x, y - attribs.y };
        }
    }

    vec2i LinuxWindow::getSize() const {
        XWindowAttributes attribs{};
        XGetWindowAttributes(display, window, &attribs);
        return { attribs.width, attribs.height };
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

                    if(XEventsQueued(display, QueuedAlready)) {
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
                    if(xevent.xbutton.button == 4) {
                        mouseDispatcher.onWheelDelta(CLV_WHEEL_DELTA, vec2i{ xevent.xbutton.x, xevent.xbutton.y });
                    } else if(xevent.xbutton.button == 5) {
                        mouseDispatcher.onWheelDelta(-CLV_WHEEL_DELTA, vec2i{ xevent.xbutton.x, xevent.xbutton.y });
                    } else {
                        mouseDispatcher.onButtonPressed(static_cast<MouseButton>(xevent.xbutton.button), vec2i{ xevent.xbutton.x, xevent.xbutton.y });
                    }
                    break;

                case ButtonRelease:
                    mouseDispatcher.onButtonReleased(static_cast<MouseButton>(xevent.xbutton.button), vec2i{ xevent.xbutton.x, xevent.xbutton.y });
                    break;

                //Window
                case ConfigureNotify: {
                    XConfigureEvent xce{ xevent.xconfigure };
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