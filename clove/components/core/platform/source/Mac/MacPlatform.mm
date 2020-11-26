#import "Clove/Platform/Platform.hpp"

#import "Clove/Platform/Mac/MacWindow.hpp"

namespace garlic::clove{
	// MacPlatform::MacPlatform(){
	// 	[NSApplication sharedApplication];
	// 	[NSApp finishLaunching];
		
	// 	//This makes it get treated like an app
	// 	[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
	// }

	std::unique_ptr<Window> Platform::createWindow(WindowDescriptor const& descriptor){
        return std::make_unique<MacWindow>(descriptor);
    }
}
