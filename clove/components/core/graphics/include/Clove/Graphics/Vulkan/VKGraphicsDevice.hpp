#pragma once

#include "Clove/Graphics/GhaDevice.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include <any>
#include <memory>

namespace garlic::clove {
    class VKGraphicsFactory;
}

namespace garlic::clove {
    class VKGraphicsDevice : public GhaDevice {
        //VARIABLES
    private:
        DevicePointer devicePtr;

        std::shared_ptr<VKGraphicsFactory> factory;

        //FUNCTIONS
    public:
        VKGraphicsDevice() = delete;
        VKGraphicsDevice(std::any nativeWindow);

        VKGraphicsDevice(VKGraphicsDevice const &other) = delete;
        VKGraphicsDevice(VKGraphicsDevice &&other) noexcept;

        VKGraphicsDevice &operator=(VKGraphicsDevice const &other) = delete;
        VKGraphicsDevice &operator=(VKGraphicsDevice &&other) noexcept;

        ~VKGraphicsDevice();

        std::shared_ptr<GhaFactory> getGraphicsFactory() const override;

        void waitForIdleDevice() override;

        Limits getLimits() const override;
    };
}