#pragma once

#include "Clove/Graphics/GhaDevice.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

#include "Clove/Graphics/Vulkan/VulkanFactory.hpp"

#include <any>
#include <memory>

namespace clove {
    class VulkanFactory;
}

namespace clove {
    class VulkanDevice : public GhaDevice {
        //TYPES
    public:
#if CLOVE_GHA_VALIDATION
        using FactoryType = VulkanFactory;
#endif

        //VARIABLES
    private:
        DevicePointer devicePtr;

        std::unique_ptr<VulkanFactory> factory;

        //FUNCTIONS
    public:
        VulkanDevice() = delete;
        VulkanDevice(std::any nativeWindow);

        VulkanDevice(VulkanDevice const &other) = delete;
        VulkanDevice(VulkanDevice &&other) noexcept;

        VulkanDevice &operator=(VulkanDevice const &other) = delete;
        VulkanDevice &operator=(VulkanDevice &&other) noexcept;

        ~VulkanDevice();

        GhaFactory *getGraphicsFactory() const override;

        void waitForIdleDevice() override;

        Info getInfo() const override;
        Limits getLimits() const override;
    };
}