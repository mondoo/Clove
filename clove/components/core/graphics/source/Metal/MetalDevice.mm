#include "Clove/Graphics/Metal/MetalDevice.hpp"

#include "Clove/Graphics/Metal/MetalFactory.hpp"

namespace garlic::clove {
	MetalDevice::MetalDevice(std::any nativeWindow) {
		//TODO
	}
	
	MetalDevice::MetalDevice(MetalDevice &&other) noexcept = default;

	MetalDevice &MetalDevice::operator=(MetalDevice &&other) noexcept = default;

	MetalDevice::~MetalDevice() = default;
	
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
