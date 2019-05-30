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

	void LinuxWindow::setEventCallbackFunction(const EventCallbackFn& callback){
        eventCallback = callback;
    }
		
	void* LinuxWindow::getNativeWindow() const{
        return display;
    }

	unsigned int LinuxWindow::getWidth() const{
        return 0u;
    }

	unsigned int LinuxWindow::getHeight() const{
        return 0u;
    }

	void LinuxWindow::setVSync(bool enabled){

    }

	bool LinuxWindow::isVSync() const{
        return false;
    }

#if CLV_PLATFORM_LINUX
	Window* Window::create(const WindowProps& props){
		return new LinuxWindow(props);
	}
#endif
}