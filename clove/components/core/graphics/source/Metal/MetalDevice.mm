#include "Clove/Graphics/Metal/MetalDevice.hpp"

#include "Clove/Graphics/Metal/MetalFactory.hpp"

namespace garlic::clove {
	MetalDevice::MetalDevice(std::any nativeWindow) {
		@autoreleasepool {
			//Create device
			device = MTLCreateSystemDefaultDevice();
		
			//Create view
			NSWindow *nsWindow{ std::any_cast<NSWindow *>(nativeWindow) };
			view = [[MTKView alloc] initWithFrame:[nsWindow frame]];
			[view setDevice:device];
			[view setClearColor:MTLClearColorMake(0.0, 0.4, 0.21, 1.0)];
		
			[nsWindow setContentView:view];
		
			factory = std::make_shared<MetalFactory>([device retain], [view retain]);
		}
	}
	
	MetalDevice::MetalDevice(MetalDevice &&other) noexcept = default;

	MetalDevice &MetalDevice::operator=(MetalDevice &&other) noexcept = default;

	MetalDevice::~MetalDevice() {
		[device release];
	}
	
	std::shared_ptr<GhaFactory> MetalDevice::getGraphicsFactory() const {
		return factory;
	}

	void waitForIdleDevice() {
		//No op
	}

	GhaDevice::Limits MetalDevice::getLimits() const {
		//TODO
		return {};
	}
}
