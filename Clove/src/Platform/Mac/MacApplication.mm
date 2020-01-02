#import "Clove/Platform/Mac/CloveMac.h"
#import "Clove/Platform/Mac/MacApplication.hpp"

#import "Clove/Platform/Mac/MacWindow.hpp"

namespace clv::plt{
	MacApplication::MacApplication(gfx::API api)
		: Application(api){
		[NSApplication sharedApplication];
		[NSApp finishLaunching];
		
		//This makes it get treated like an app
		[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
	}
	
	std::shared_ptr<Window> MacApplication::createWindow(const WindowProps& props){
        return std::make_shared<MacWindow>(props);
    }
	
	std::shared_ptr<Window> MacApplication::createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size){
		return std::make_shared<MacWindow>(parentWindow, position, size);
	}
}
