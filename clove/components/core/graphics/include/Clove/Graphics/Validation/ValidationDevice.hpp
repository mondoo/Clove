#pragma once

#include "Clove/Graphics/GhaDevice.hpp"

#include <memory>

namespace clove {
    class ValidationFactory;
}

namespace clove {
    class ValidationDevice : public GhaDevice {
        //VARIABLES
    private:
        std::unique_ptr<GhaDevice> internalDevice{};
        std::unique_ptr<ValidationFactory> factory{};

        //FUNCTIONS
    public:
        ValidationDevice() = delete;
        ValidationDevice(std::unique_ptr<GhaDevice> device);

        ValidationDevice(ValidationDevice const &other) = delete;
        ValidationDevice(ValidationDevice &&other) noexcept;

        ValidationDevice &operator=(ValidationDevice const &other) = delete;
        ValidationDevice &operator=(ValidationDevice &&other) noexcept;

        GhaFactory *getGraphicsFactory() const override;

        void waitForIdleDevice() override;

        Limits getLimits() const override;
    };
}