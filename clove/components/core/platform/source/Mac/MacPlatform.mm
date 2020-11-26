#import "Clove/Platform/Mac/MacPlatform.hpp"

#import "Clove/Platform/Mac/MacWindow.hpp"

namespace garlic::clove{
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
}
