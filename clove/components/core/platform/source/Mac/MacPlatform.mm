#import "Clove/Platform/Platform.hpp"

#import "Clove/Platform/Mac/MacWindow.hpp"

namespace garlic::clove{
	std::unique_ptr<Window> Platform::createWindow(WindowDescriptor const& descriptor){
        return std::make_unique<MacWindow>(descriptor);
    }

	void Platform::showCursor(bool show) {
        
    }
}
