#include "clvpch.hpp"
#include "LinuxWindow.hpp"

#include "Clove/Graphics/Renderer.hpp"
#include "Clove/Events/ApplicationEvent.hpp"
#include "Clove/Events/MouseEvent.hpp"
#include "Clove/Events/KeyEvent.hpp"

namespace clv{
    LinuxWindow::~LinuxWindow(){
        XDestroyWindow(display, windowID);
        XCloseDisplay(display);
    }

    LinuxWindow::LinuxWindow(const WindowProps& props){
        initialiseWindow(props, gfx::API::OpenGL4);
    }

    LinuxWindow::LinuxWindow(const WindowProps& props, gfx::API api){
        initialiseWindow(props, api);
    }

    void LinuxWindow::beginFrame(){
        //clear frame
        
        XNextEvent(display, &xevent);
        if(xevent.type == ClientMessage){
            //if(xevent.xclient.data.l[0] == wmDeleteMessage){
            //    WindowCloseEvent event;
			//	eventCallback(event);
            //}
        }else if(xevent.type == DestroyNotify){
            WindowCloseEvent event;
			eventCallback(event);
        }
    }

	void LinuxWindow::endFrame(){
        //draw
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
        display = XOpenDisplay(nullptr);

        if(display == nullptr){
            CLV_LOG_ERROR("Could not open display");
            return;
        }

        screenID = DefaultScreen(display);
        windowID = XCreateSimpleWindow(
            display, 
            RootWindow(display, screenID), 
            0, 0, props.width, props.height, 
            1, BlackPixel(display, screenID), WhitePixel(display, screenID)
        );

        XStoreName(display, windowID, props.title.c_str());
    
        XSelectInput(display, windowID, ExposureMask | KeyPressMask);
        XMapWindow(display, windowID);
        
        CLV_LOG_INFO("Created X11 Window");
    }

#if CLV_PLATFORM_LINUX
	Window* Window::create(const WindowProps& props){
		return new LinuxWindow(props);
	}

    Window* Window::create(const WindowProps& props, gfx::API api){
        return new LinuxWindow(props, api);
    }
#endif
}