#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

namespace clv::gfx::vk {
    static void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if(func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    DevicePointer::DevicePointer(VkInstance instance, VkSurfaceKHR surface, VkDevice logicalDevice, VkDebugUtilsMessengerEXT debugMessenger)
        : instance(instance)
        , surface(surface)
        , logicalDevice(logicalDevice)
        , debugMessenger(debugMessenger) {
        counter = new uint32_t(1);
    }

    DevicePointer::DevicePointer(const DevicePointer& other)
        : instance(other.instance)
        , surface(other.surface)
        , logicalDevice(other.logicalDevice)
        , debugMessenger(other.debugMessenger) {
        counter = other.counter;
        ++(*counter);
    }

    DevicePointer::DevicePointer(DevicePointer&& other) noexcept
        : instance(other.instance)
        , surface(other.surface)
        , logicalDevice(other.logicalDevice)
        , debugMessenger(other.debugMessenger) {
        counter = other.counter;
        ++(*counter);
    }

    DevicePointer& DevicePointer::operator=(const DevicePointer& other) {
        instance       = other.instance;
        surface        = other.surface;
        logicalDevice  = other.logicalDevice;
        debugMessenger = other.debugMessenger;

        counter = other.counter;
        ++(*counter);
    }

    DevicePointer& DevicePointer::operator=(DevicePointer&& other) noexcept {
        instance       = other.instance;
        surface        = other.surface;
        logicalDevice  = other.logicalDevice;
        debugMessenger = other.debugMessenger;

        counter = other.counter;
        ++(*counter);
    }

    DevicePointer::~DevicePointer() {
        if(--(*counter) == 0) {
#if GARLIC_DEBUG
            destroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
#endif

            vkDestroyDevice(logicalDevice, nullptr);
            vkDestroySurfaceKHR(instance, surface, nullptr);

            vkDestroyInstance(instance, nullptr);

            delete counter;
        }
    }

    VkDevice DevicePointer::get() const {
        return logicalDevice;
    }
}