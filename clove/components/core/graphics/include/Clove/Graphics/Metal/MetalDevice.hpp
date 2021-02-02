#pragma once

#include "Clove/Graphics/GhaDevice.hpp"

#include <any>
#include <memory>

namespace garlic::clove {
	class MetalFactory;
}

namespace garlic::clove {
	class MetalDevice : public GhaDevice {
		//VARIABLES
	private:
		std::shared_ptr<MetalFactory> factory;
		
		//FUNCTIONS
	public:
		MetalDevice() = delete;
		MetalDevice(std::any nativeWindow);
		
		MetalDevice(MetalDevice const &other) = delete;
		MetalDevice(MetalDevice &&other) noexcept;

		MetalDevice &operator=(MetalDevice const &other) = delete;
		MetalDevice &operator=(MetalDevice &&other) noexcept;

		~MetalDevice();
		
		std::shared_ptr<GhaFactory> getGraphicsFactory() const override;

		void waitForIdleDevice() override;

		Limits getLimits() const override;
	};
}
