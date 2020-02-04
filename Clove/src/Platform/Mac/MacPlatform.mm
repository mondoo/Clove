#import "Clove/Platform/Mac/MacPlatform.hpp"

#import "Clove/Platform/Mac/MacWindow.hpp"

namespace clv::plt{
	MacPlatform::MacPlatform(){
		[NSApplication sharedApplication];
		[NSApp finishLaunching];
		
		//This makes it get treated like an app
		[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
	}

	MacPlatform::MacPlatform(MacPlatform&& other) noexcept = default;

	MacPlatform& MacPlatform::operator=(MacPlatform&& other) noexcept = default;

	MacPlatform::~MacPlatform() = default;
	
	std::shared_ptr<Window> MacPlatform::createWindow(const WindowDescriptor& props){
        return std::make_shared<MacWindow>(props);
    }
	
	std::shared_ptr<Window> MacPlatform::createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size, const gfx::API api){
		return std::make_shared<MacWindow>(parentWindow, position, size, api);
	}
}
