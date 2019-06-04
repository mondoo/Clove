#include "clvpch.hpp"
#include "LinuxWindow.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Events/ApplicationEvent.hpp"
#include "Clove/Events/MouseEvent.hpp"
#include "Clove/Events/KeyEvent.hpp"

#include <X11/Xresource.h>

//TODO: Move to renderer
//#include <GL/gl.h>
//#include <GL/glx.h> 
typedef GLXContext (*glxCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, bool, const int*);

static bool isExtensionSupported(const char* extList, const char* extension){
    const char* start;
    const char* where;
    const char* terminator;

    where = strchr(extension, ' ');
    if(where || *extension == '\0'){
        return false;
    }

    for(start=extList;;){
        where = strstr(start, extension);
        if(!where){
            break;
        }

        terminator = where + strlen(extension);

        if(where == start || *(where - 1) == ' '){
            if(*terminator == ' ' || *terminator == '\0'){
                return true;
            }
        }

        start = terminator;
    }

    return false;
}

//~

namespace clv{
    LinuxWindow::~LinuxWindow(){
        //TODO: Move to renderer
        glXDestroyContext(display, context);
        //~

        XFree(screen);
        XFreeColormap(display, windowAttribs.colormap);
        XDestroyWindow(display, window);
        XCloseDisplay(display);
    }

    LinuxWindow::LinuxWindow(const WindowProps& props){
        initialiseWindow(props, gfx::API::OpenGL4);
    }

    LinuxWindow::LinuxWindow(const WindowProps& props, gfx::API api){
        initialiseWindow(props, api);
    }

    void LinuxWindow::beginFrame(){
        renderer->clear();
        
        if(XPending(display) > 0){
            KeySym xkeysym = 0;

            XNextEvent(display, &xevent);
            switch(xevent.type){
                case ClientMessage:
                    if(xevent.xclient.data.l[0] == atomWmDeleteWindow){
                        WindowCloseEvent event;
				        eventCallback(event);
                    }
                    break;
                case DestroyNotify:
                    {
                        WindowCloseEvent event;
				        eventCallback(event);
                    }
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
                    }else if(xevent.xbutton.button == 5){
                        mouse.onWheelDelta(-CLV_WHEEL_DELTA, xevent.xbutton.x, xevent.xbutton.y);                    
                    }else{
                        mouse.onButtonPressed(static_cast<MouseButton>(xevent.xbutton.button), xevent.xbutton.x, xevent.xbutton.y);
                    }
                    break;

                case ButtonRelease:
                    mouse.onButtonReleased(static_cast<MouseButton>(xevent.xbutton.button), xevent.xbutton.x, xevent.xbutton.y);
                    break;
            }
        }
    }

	void LinuxWindow::endFrame(){
        renderer->draw();

        //TEMP: manually swapping buffers here:
        glXSwapBuffers(display, window);
        //~
    }

	void* LinuxWindow::getNativeWindow() const{
        return display;
    }

	void LinuxWindow::setVSync(bool enabled){

    }

	bool LinuxWindow::isVSync() const{
        return false;
    }

    void LinuxWindow::initialiseWindow(const WindowProps& props, gfx::API api){
        data.title = props.title;
		data.width = props.width;
		data.height = props.height;
        
        display = XOpenDisplay(nullptr); //makes the connection to the client, where to display the window

        if(!display){
            //TODO: Exception
            CLV_LOG_ERROR("Could not open display");
            return;
        }

        screen = DefaultScreenOfDisplay(display); //Get the screen of the display
        screenID = DefaultScreen(display);

        //We need to create the window after we've defined the openGL attributes because we need the data from it
        //TODO: Move to renderer
        GLint majorGLX;
        GLint minorGLX;
        glXQueryVersion(display, &majorGLX, &minorGLX);
        if(majorGLX <= 1 && minorGLX < 2){
            //TODO: Exception
            CLV_LOG_CRITICAL("GLX 1.2 or greater is required");
            XCloseDisplay(display);
            return;
        }
        
        GLint glxAttribs[] = {
            GLX_X_RENDERABLE,   true,
            GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
            GLX_RENDER_TYPE,    GLX_RGBA_BIT,
            GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
            GLX_RED_SIZE,       8,
            GLX_GREEN_SIZE,     8,
            GLX_BLUE_SIZE,      8,
            GLX_ALPHA_SIZE,     8,
            GLX_DEPTH_SIZE,     24,
            GLX_STENCIL_SIZE,   8,
            GLX_DOUBLEBUFFER,   true,
            0,
        };

        int fbcount = 0;
        GLXFBConfig* fbc = glXChooseFBConfig(display, screenID, glxAttribs, &fbcount);
        if(!fbc){
            //TODO: Exception
            CLV_LOG_CRITICAL("Failed to retrieve frame buffer");
            return;
        }

        //Get the frame buffer with the most samples per pixel
        int bestfbc = -1;
        int worstfbc = -1;
        int bestNumSamp = 0;
        int worstNumSamp = 999;
        for(int i = 0; i < fbcount; ++i){
            XVisualInfo* vi = glXGetVisualFromFBConfig(display, fbc[i]);
            if(vi){
                int sampBuff;
                int samples;
                glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLE_BUFFERS, &sampBuff);
                glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLES, &samples);

                if(bestfbc < 0 || (sampBuff && samples > bestNumSamp)){
                    bestfbc = i;
                    bestNumSamp = samples;
                }
                if(worstfbc < 0 || !sampBuff || samples < worstNumSamp){
                    worstfbc = i;
                }
                worstNumSamp = samples;
            }
            XFree(vi);
        }

        GLXFBConfig glFBC = fbc[bestfbc];
        XFree(fbc);
        //~

        visual = glXGetVisualFromFBConfig(display, glFBC);
        if(!visual){
            //TODO: Exception
            CLV_LOG_CRITICAL("Could not create visual");
            return;
        }

        if(screenID != visual->screen){
            //TODO: Exception
            CLV_LOG_CRITICAL("Screen ID does not match visual->screen");
            return;
        }

        windowAttribs.border_pixel = BlackPixel(display, screenID);
        windowAttribs.background_pixel = WhitePixel(display, screenID);
        windowAttribs.override_redirect = true;
        windowAttribs.colormap = XCreateColormap(display, RootWindow(display, screenID), visual->visual, AllocNone);
        windowAttribs.event_mask = ExposureMask;
        window = XCreateWindow(display, RootWindow(display, screenID), 
                                    0, 0, data.width, data.height,
                                    0, visual->depth, InputOutput, visual->visual,
                                    CWBackPixel | CWColormap | CWBorderPixel | CWEventMask, 
                                    &windowAttribs);

        //window = XCreateSimpleWindow(display, RootWindow(display, screenID), 
        //                            0, 0, data.width, data.height,
        //                            1, BlackPixel(display, screenID), WhitePixel(display, screenID));
        
        //TODO: Move to renderer
        //GLXContext context = glXCreateContext(display, visual, nullptr, GL_TRUE);
        //glXMakeCurrent(display, window, context);
        
        //GLXContext tempContext = glXCreateNewContext(display, glFBC, GLX_RGBA_TYPE, 0, true);
        //glXMakeCurrent(display, window, tempContext);

        //Remap the delete window message so we can gracefully close the application
        atomWmDeleteWindow = XInternAtom(display, "WM_DELTE_WNDOW", false);
        XSetWMProtocols(display, window, &atomWmDeleteWindow, 1);

        //glxCreateContextAttribsARBProc glxCreateContextAttribsARB = 0;
        PFNGLXCREATECONTEXTATTRIBSARBPROC glxCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddressARB((const GLubyte*) "glxCreateContextAttribsARB");
        if(!glxCreateContextAttribsARB){
            //TODO: Exception
            CLV_LOG_CRITICAL("glxCreateContextAttribsARB not found!");
            return;
        }

        const char* glxExts = glXQueryExtensionsString(display, screenID);

        int contextAttribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
            GLX_CONTEXT_MINOR_VERSION_ARB, 6,
            GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            0
        };

        if(!isExtensionSupported(glxExts, "GLX_ARB_create_context")){
            CLV_LOG_DEBUG("Extension isn't supported");
            context = glXCreateNewContext(display, glFBC, GLX_RGBA_TYPE, 0, true);
        }else{
            CLV_LOG_DEBUG("Extension is supported");
            context = glxCreateContextAttribsARB(display, glFBC, 0, true, contextAttribs);
        }

        if (!context){
            CLV_LOG_WARN("Could not create context, platform might not support opengl4.6");
            context = glXCreateNewContext(display, glFBC, GLX_RGBA_TYPE, 0, true);
        }

        XSync(display, false); //Passing true here flushes the event queue
        
        if(!glXIsDirect(display, context)){
            CLV_LOG_TRACE("Indirect GLX rendering context obtained");
        }else{
            CLV_LOG_TRACE("Direct GLX rendering context obtained");
        }

        //glXMakeCurrent(nullptr, 0, nullptr);
        //glXDestroyContext(display, tempContext);

        CLV_LOG_TRACE("Making context current");
        glXMakeCurrent(display, window, context);
        //~

        const long keyboardMask = KeyPressMask | KeyReleaseMask | KeymapStateMask;
        const long mouseMask = PointerMotionMask | ButtonPressMask | ButtonReleaseMask | EnterWindowMask | LeaveWindowMask;

        XSelectInput(display, window, keyboardMask | mouseMask);

        XStoreName(display, window, data.title.c_str());
        
        XClearWindow(display, window);
        XMapRaised(display, window);

        renderer = gfx::Renderer::createRenderer(*this, api);

        CLV_LOG_INFO("Created X11 Window");
    }

	Window* Window::create(const WindowProps& props){
		return new LinuxWindow(props);
	}

    Window* Window::create(const WindowProps& props, gfx::API api){
        return new LinuxWindow(props, api);
    }
}