#pragma once

#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
    /**
     * @brief Wraps a Vulkan device / instance to help control the lifetime of those objects.
     */
    class DevicePointer {
        //VARIABLES
    private:
        VkDevice logicalDevice{ VK_NULL_HANDLE };

        VkInstance instance{ VK_NULL_HANDLE };
        VkSurfaceKHR surface{ VK_NULL_HANDLE };
        VkDebugUtilsMessengerEXT debugMessenger{ VK_NULL_HANDLE };

        uint32_t* counter{ nullptr };

        //FUNCTIONS
    public:
        DevicePointer() = delete;
        DevicePointer(VkInstance instance, VkSurfaceKHR surface, VkDevice logicalDevice, VkDebugUtilsMessengerEXT debugMessenger);

        DevicePointer(const DevicePointer& other);
        DevicePointer(DevicePointer&& other) noexcept;

        DevicePointer& operator=(const DevicePointer& other);
        DevicePointer& operator=(DevicePointer&& other) noexcept;

        ~DevicePointer();

        VkDevice get() const;
    };
}