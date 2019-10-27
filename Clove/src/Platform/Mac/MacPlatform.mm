#import "Platform/Mac/CloveMac.h"
#import "Clove/Platform/Platform.hpp"

#import "Platform/Mac/MacWindow.hpp"

namespace clv::plt{
	void Platform::prepare(){
		[NSApplication sharedApplication];
		[NSApp finishLaunching];
		
		//This makes it get treated like an app
		[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
	}
	
	std::unique_ptr<Window> Platform::createWindow(const WindowProps& props){
        return std::make_unique<MacWindow>(props);
    }

    std::unique_ptr<Window> Platform::createWindow(const WindowProps& props, gfx::API api){
        return std::make_unique<MacWindow>(props, api);
    }
}
