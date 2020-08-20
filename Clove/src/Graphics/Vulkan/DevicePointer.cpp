#include "Clove/Graphics/Vulkan/DevicePointer.hpp"

namespace clv::gfx::vk {
    static void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if(func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    DevicePointer::DevicePointer() = default;

    DevicePointer::DevicePointer(VkInstance instance, VkSurfaceKHR surface, VkDevice logicalDevice, VkDebugUtilsMessengerEXT debugMessenger)
        : instance(instance)
        , surface(surface)
        , logicalDevice(logicalDevice)
        , debugMessenger(debugMessenger) {
        counter = new uint32_t(1);
    }

    DevicePointer::DevicePointer(const DevicePointer& other) {
        release();

        instance       = other.instance;
        surface        = other.surface;
        logicalDevice  = other.logicalDevice;
        debugMessenger = other.debugMessenger;

        counter = other.counter;
        ++(*counter);
    }

    DevicePointer::DevicePointer(DevicePointer&& other) noexcept {
        instance       = std::move(other.instance);
        surface        = std::move(other.surface);
        logicalDevice  = std::move(other.logicalDevice);
        debugMessenger = std::move(other.debugMessenger);

        counter = std::move(other.counter);
        //++(*counter);
    }

    DevicePointer& DevicePointer::operator=(const DevicePointer& other) {
        release();

        instance       = other.instance;
        surface        = other.surface;
        logicalDevice  = other.logicalDevice;
        debugMessenger = other.debugMessenger;

        counter = other.counter;
        ++(*counter);

        return *this;
    }

    DevicePointer& DevicePointer::operator=(DevicePointer&& other) noexcept {
        instance       = std::move(other.instance);
        surface        = std::move(other.surface);
        logicalDevice  = std::move(other.logicalDevice);
        debugMessenger = std::move(other.debugMessenger);

        counter = std::move(other.counter);
        //++(*counter);

        return *this;
    }

    DevicePointer::~DevicePointer() {
        release();
    }

    void DevicePointer::release() {
        if(counter != nullptr && --(*counter) == 0) {
#if GARLIC_DEBUG
            destroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
#endif

            vkDestroyDevice(logicalDevice, nullptr);
            vkDestroySurfaceKHR(instance, surface, nullptr);

            vkDestroyInstance(instance, nullptr);

            delete counter;
        }
    }
}