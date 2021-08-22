#include "Clove/Graphics/Validation/ValidationDevice.hpp"

#include "Clove/Graphics/Validation/ValidationFactory.hpp"
#include "Clove/Graphics/Validation/ValidationLog.hpp"

namespace clove {
    ValidationDevice::ValidationDevice(std::unique_ptr<GhaDevice> device)
        : internalDevice{ std::move(device) } {
        CLOVE_LOG(LOG_CATEGORY_CLOVE_GHA_VALIDATION, LogLevel::Debug, "GHA Validation enabled.");
        factory = std::make_unique<ValidationFactory>(internalDevice->getGraphicsFactory());
    }

    ValidationDevice::ValidationDevice(ValidationDevice &&other) noexcept = default;

    ValidationDevice &ValidationDevice::operator=(ValidationDevice &&other) noexcept = default;

    GhaFactory *ValidationDevice::getGraphicsFactory() const {
        return factory.get();
    }

    void ValidationDevice::waitForIdleDevice() {
        internalDevice->waitForIdleDevice();
    }

    GhaDevice::Limits ValidationDevice::getLimits() const {
        return internalDevice->getLimits();
    }
}