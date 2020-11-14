#include "Clove/Platform/Linux/LinuxWindow.hpp"

#include "Clove/Graphics/Graphics.hpp"
#include "Clove/Graphics/GraphicsFactory.hpp"

#include <Root/Definitions.hpp>
#include <Root/Log/Log.hpp>

namespace clv::plt {
    LinuxWindow::LinuxWindow(WindowDescriptor const &descriptor) {
        GARLIC_LOG(garlicLogContext, garlic::LogLevel::Trace, "Creating window: {0} ({1}, {2})", descriptor.title, descriptor.width, descriptor.height);

        nativeWindow.display = XOpenDisplay(nullptr);//makes the connection to the client, where to display the window

        if(!nativeWindow.display) {
            //TODO: Exception
            GARLIC_LOG(garlicLogContext, garlic::LogLevel::Error, "Could not open display");
            return;
        }

        screen   = DefaultScreenOfDisplay(nativeWindow.display);//Get the screen of the display
        screenID = DefaultScreen(nativeWindow.display);

        windowAttribs                   = {};
        windowAttribs.border_pixel      = BlackPixel(nativeWindow.display, screenID);
        windowAttribs.background_pixel  = WhitePixel(nativeWindow.display, screenID);
        windowAttribs.override_redirect = true;
        windowAttribs.colormap          = XCreateColormap(nativeWindow.display, RootWindow(nativeWindow.display, screenID), screen->root_visual, AllocNone);
        windowAttribs.event_mask        = ExposureMask;

        nativeWindow.window = XCreateWindow(nativeWindow.display, RootWindow(nativeWindow.display, screenID), 0, 0, descriptor.width, descriptor.height, 0, screen->depths[0].depth, InputOutput, screen->root_visual, CWBackPixel | CWColormap | CWBorderPixel | CWEventMask, &windowAttribs);

        //Remap the delete window message so we can gracefully close the application
        atomWmDeleteWindow = XInternAtom(nativeWindow.display, "WM_DELETE_WINDOW", false);
        XSetWMProtocols(nativeWindow.display, nativeWindow.window, &atomWmDeleteWindow, 1);

        XSync(nativeWindow.display, false);//Passing true here flushes the event queue

        long const keyboardMask{ KeyPressMask | KeyReleaseMask | KeymapStateMask };
        long const mouseMask{ PointerMotionMask | ButtonPressMask | ButtonReleaseMask | EnterWindowMask | LeaveWindowMask };

        XSelectInput(nativeWindow.display, nativeWindow.window, keyboardMask | mouseMask | StructureNotifyMask);

        XStoreName(nativeWindow.display, nativeWindow.window, descriptor.title.c_str());

        XClearWindow(nativeWindow.display, nativeWindow.window);
        XMapRaised(nativeWindow.display, nativeWindow.window);

        open = true;

        GARLIC_LOG(garlicLogContext, garlic::LogLevel::Debug, "Window created");
    }

    LinuxWindow::LinuxWindow(Window const &parentWindow, mth::vec2i const &position, mth::vec2i const &size) {
        GARLIC_LOG(garlicLogContext, garlic::LogLevel::Trace, "Creating child window: ({1}, {2})", size.x, size.y);

        NativeWindow const nativeParentWindow = std::any_cast<NativeWindow>(parentWindow.getNativeWindow());

        nativeWindow.display = XOpenDisplay(nullptr);//makes the connection to the client, where to display the window

        if(!nativeWindow.display) {
            //TODO: Exception
            GARLIC_LOG(garlicLogContext, garlic::LogLevel::Error, "Could not open display");
            return;
        }

        screen   = DefaultScreenOfDisplay(nativeWindow.display);//Get the screen of the display
        screenID = DefaultScreen(nativeWindow.display);

        windowAttribs                   = {};
        windowAttribs.border_pixel      = BlackPixel(nativeWindow.display, screenID);
        windowAttribs.background_pixel  = WhitePixel(nativeWindow.display, screenID);
        windowAttribs.override_redirect = true;
        windowAttribs.colormap          = XCreateColormap(nativeWindow.display, RootWindow(nativeWindow.display, screenID), screen->root_visual, AllocNone);
        windowAttribs.event_mask        = ExposureMask;

        nativeWindow.window = XCreateWindow(nativeWindow.display, nativeParentWindow.window, position.x, position.y, size.x, size.y, 0, screen->depths[0].depth, InputOutput, screen->root_visual, CWBackPixel | CWColormap | CWBorderPixel | CWEventMask, &windowAttribs);

        //Remap the delete window message so we can gracefully close the application
        atomWmDeleteWindow = XInternAtom(nativeWindow.display, "WM_DELETE_WINDOW", false);
        XSetWMProtocols(nativeWindow.display, nativeWindow.window, &atomWmDeleteWindow, 1);

        XSync(nativeWindow.display, false);//Passing true here flushes the event queue

        long const keyboardMask{ KeyPressMask | KeyReleaseMask | KeymapStateMask };
        long const mouseMask{ PointerMotionMask | ButtonPressMask | ButtonReleaseMask | EnterWindowMask | LeaveWindowMask };

        XSelectInput(nativeWindow.display, nativeWindow.window, keyboardMask | mouseMask | StructureNotifyMask);

        XClearWindow(nativeWindow.display, nativeWindow.window);
        XMapRaised(nativeWindow.display, nativeWindow.window);

        open = true;

        GARLIC_LOG(garlicLogContext, garlic::LogLevel::Debug, "Window created");
    }

    LinuxWindow::~LinuxWindow() {
        XFreeColormap(nativeWindow.display, windowAttribs.colormap);
        XDestroyWindow(nativeWindow.display, nativeWindow.window);
        XCloseDisplay(nativeWindow.display);
    }

    std::any LinuxWindow::getNativeWindow() const {
        return nativeWindow;
    }

    mth::vec2i LinuxWindow::getPosition() const {
        ::Window rootWindow;
        int32_t posX;
        int32_t posY;
        uint32_t width;
        uint32_t height;
        uint32_t borderWidth;
        uint32_t depth;

        if(XGetGeometry(nativeWindow.display, nativeWindow.window, &rootWindow, &posX, &posY, &width, &height, &borderWidth, &depth) != 0) {
            return { posX, posY };
        } else {
            GARLIC_ASSERT(false, "Could not get window geometry");
            return { 0, 0 };
        }
    }

    mth::vec2i LinuxWindow::getSize() const {
        ::Window rootWindow;
        int32_t posX;
        int32_t posY;
        uint32_t width;
        uint32_t height;
        uint32_t borderWidth;
        uint32_t depth;

        if(XGetGeometry(nativeWindow.display, nativeWindow.window, &rootWindow, &posX, &posY, &width, &height, &borderWidth, &depth) != 0) {
            return { width, height };
        } else {
            GARLIC_ASSERT(false, "Could not get window geometry");
            return { 0, 0 };
        }
    }

    void LinuxWindow::moveWindow(mth::vec2i const &position) {
        XMoveWindow(nativeWindow.display, nativeWindow.window, position.x, position.y);
    }

    void LinuxWindow::resizeWindow(mth::vec2i const &size) {
        XResizeWindow(nativeWindow.display, nativeWindow.window, size.x, size.y);
    }

    bool LinuxWindow::isOpen() const {
        return open;
    }

    void LinuxWindow::close() {
        if(onWindowCloseDelegate.isBound()) {
            onWindowCloseDelegate.broadcast();
        }
        open = false;
        XCloseDisplay(nativeWindow.display);
    }

    void LinuxWindow::processInput() {
        if(XPending(nativeWindow.display) > 0) {
            KeySym xkeysym = 0;

            XNextEvent(nativeWindow.display, &xevent);
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

                case KeyRelease: {
                    bool isRepeat = false;

                    if(XEventsQueued(nativeWindow.display, QueuedAlready)) {
                        XEvent nextEvent;
                        XPeekEvent(nativeWindow.display, &nextEvent);

                        isRepeat = nextEvent.type == KeyPress && nextEvent.xkey.keycode == xevent.xkey.keycode;
                        if(isRepeat) {
                            //Consume the next KeyPress event (as that is auto repeat)
                            XNextEvent(nativeWindow.display, &xevent);
                        }
                    }

                    if(!isRepeat || keyboard.isAutoRepeatEnabled()) {
                        xkeysym = XLookupKeysym(&xevent.xkey, 0);
                        keyboard.onKeyReleased(static_cast<Key>(xkeysym));
                    }
                } break;

                    //TODO: Char (I don't think Xlib has a 'typed' event)

                case EnterNotify:
                    mouse.onMouseEnter();
                    break;

                case LeaveNotify:
                    mouse.onMouseLeave();
                    break;

                case ButtonPress:
                    if(xevent.xbutton.button == 4) {
                        mouse.onWheelDelta(CLV_WHEEL_DELTA, xevent.xbutton.x, xevent.xbutton.y);
                    } else if(xevent.xbutton.button == 5) {
                        mouse.onWheelDelta(-CLV_WHEEL_DELTA, xevent.xbutton.x, xevent.xbutton.y);
                    } else {
                        mouse.onButtonPressed(static_cast<MouseButton>(xevent.xbutton.button), xevent.xbutton.x, xevent.xbutton.y);
                    }
                    break;

                case ButtonRelease:
                    mouse.onButtonReleased(static_cast<MouseButton>(xevent.xbutton.button), xevent.xbutton.x, xevent.xbutton.y);
                    break;

                //Window
                case ConfigureNotify: {
                    XConfigureEvent xce = xevent.xconfigure;
                    if(static_cast<uint32_t>(xce.width) != prevConfigureNotifySize.x || static_cast<uint32_t>(xce.height) != prevConfigureNotifySize.y) {
                        const mth::vec2i size{ xce.width, xce.height };
                        prevConfigureNotifySize = size;
                        onWindowResize.broadcast(size);
                    }
                } break;
            }
        }
    }
}