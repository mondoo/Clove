#import "Clove/Platform/Mac/CloveMac.h"
#import "Clove/Platform/Mac/MacApplication.hpp"

#import "Clove/Platform/Mac/MacWindow.hpp"

namespace clv::plt{
	MacApplication::MacApplication(){
		[NSApplication sharedApplication];
		[NSApp finishLaunching];
		
		//This makes it get treated like an app
		[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
	}
	
	gfx::API MacApplication::getPlatformPreferedAPI(){
		return gfx::API::OpenGL4;	
	}

	std::unique_ptr<Window> MacApplication::createWindow(const WindowProps& props){
        return std::make_unique<MacWindow>(props);
    }

	std::unique_ptr<Window> MacApplication::createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size){
        CLV_ASSERT(false, "TODO: Child windows not yet supported on macOS");
		return std::make_unique<Window>();
    }
}
