#import "Platform/Mac/CloveMac.h"
#import "MacApplication.hpp"

#import "Platform/Mac/MacWindow.hpp"

namespace clv::plt{
	MacApplication::MacApplication(){
		[NSApplication sharedApplication];
		[NSApp finishLaunching];
		
		//This makes it get treated like an app
		[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
	}
	
	std::unique_ptr<Window> MacApplication::createWindow(const WindowProps& props){
        return std::make_unique<MacWindow>(props);
    }

    std::unique_ptr<Window> MacApplication::createWindow(const WindowProps& props, gfx::API api){
        return std::make_unique<MacWindow>(props, api);
    }
}
