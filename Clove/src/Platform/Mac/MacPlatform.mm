#import "Clove/Platform/Mac/MacPlatform.hpp"

#import "Clove/Platform/Mac/MacWindow.hpp"
#include "Clove/Graphics/Core/Graphics.hpp"
#include "Clove/Graphics/Core/RenderFactory.hpp"

namespace clv::plt{
	MacPlatform::MacPlatform()
	MacPlatform::MacPlatform(gfx::API api){
		[NSApplication sharedApplication];
		[NSApp finishLaunching];
		
		//This makes it get treated like an app
		[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

		graphicsFactory = gfx::initialise(api);
	}

	MacPlatform::MacPlatform(MacPlatform&& other) noexcept = default;

	MacPlatform& MacPlatform::operator=(MacPlatform&& other) noexcept = default;

	MacPlatform::~MacPlatform() = default;

	gfx::RenderFactory& MacPlatform::getGraphicsFactory(){
		return *graphicsFactory;
	}
	
	std::shared_ptr<Window> MacPlatform::createWindow(const WindowProps& props){
        return std::make_shared<MacWindow>(props);
    }
	
	std::shared_ptr<Window> MacPlatform::createChildWindow(const Window& parentWindow, const mth::vec2i& position, const mth::vec2i& size){
		return std::make_shared<MacWindow>(parentWindow, position, size);
	}
}
