#pragma once

#include "Clove/Graphics/GraphicsDevice.hpp"
#include "Clove/Graphics/Vulkan/DevicePointer.hpp"
#include "Clove/Graphics/Vulkan/VulkanTypes.hpp"

namespace clv::gfx::vk {
    class VKGraphicsFactory;
}

namespace clv::gfx::vk {
    class VKGraphicsDevice : public GraphicsDevice {
        //VARIABLES
    private:
        DevicePointer devicePtr;

        std::shared_ptr<VKGraphicsFactory> factory;

        //FUNCTIONS
    public:
        VKGraphicsDevice() = delete;
        VKGraphicsDevice(void* nativeWindow);

        VKGraphicsDevice(const VKGraphicsDevice& other) = delete;
        VKGraphicsDevice(VKGraphicsDevice&& other) noexcept;

        VKGraphicsDevice& operator=(const VKGraphicsDevice& other) = delete;
        VKGraphicsDevice& operator=(VKGraphicsDevice&& other) noexcept;

        ~VKGraphicsDevice();

        std::shared_ptr<GraphicsFactory> getGraphicsFactory() const override;

        void waitForIdleDevice() override;

        Limits getLimits() const override;
    };
}