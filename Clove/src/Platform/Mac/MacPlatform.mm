#import "Clove/Platform/Mac/CloveMac.h"
#import "Clove/Platform/Mac/MacPlatform.hpp"

#import "Clove/Platform/Mac/MacWindow.hpp"

namespace clv::plt{
	MacPlatform::MacPlatform()
		: Platform(){
		[NSApplication sharedApplication];
		[NSApp finishLaunching];
		
		//This makes it get treated like an app
		[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
	}
	
	std::shared_ptr<Window> MacPlatform::createWindow(const WindowProps& props){
        return std::make_shared<MacWindow>(props);
    }
	
	std::shared_ptr<Window> MacPlatform::createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size){
		return std::make_shared<MacWindow>(parentWindow, position, size);
	}
}
