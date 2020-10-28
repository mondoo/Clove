#include "Clove/Graphics/Vulkan/VKGraphicsDevice.hpp"

#include "Clove/Graphics/Vulkan/VKGraphicsFactory.hpp"

//TODO: Abstract away
#include "Clove/Platform/Windows/CloveWindows.hpp"

#include <Root/Definitions.hpp>
#include <Root/Log/Log.hpp>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

//TODO: Move this callback (and the set up) into VKException.hpp
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messagType,
    VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData,
    void *pUserData) {
    if((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) != 0) {
        GARLIC_LOG(garlicLogContext, garlic::LogLevel::Trace, pCallbackData->pMessage);
    } else if((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) != 0) {
        GARLIC_LOG(garlicLogContext, garlic::LogLevel::Warning, pCallbackData->pMessage);
    } else if((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) != 0) {
        GARLIC_LOG(garlicLogContext, garlic::LogLevel::Error, pCallbackData->pMessage);
    }

    return VK_FALSE;
}

namespace clv::gfx::vk {
    static VkResult createDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT const *pCreateInfo, VkAllocationCallbacks const *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if(func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    static bool checkValidationLayerSupport(std::vector<char const *> const &validationLayers) {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for(char const *layerName : validationLayers) {
            bool layerFound = false;

            for(auto const &layerProperties : availableLayers) {
                if(std::strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if(!layerFound) {
                return false;
            }
        }

        return true;
    }

    static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for(auto const &queueFamily : queueFamilies) {
            //Make sure we have the queue family that'll let us render graphics
            if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            //Make sure we have a queue family that'll let us present to a window, might not be the same family that supports graphics
            //We could enforce this by only choosing devices that have presentation support in it's graphics family
            VkBool32 presentSupport = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if(presentSupport == VK_TRUE) {
                indices.presentFamily = i;
            }

            //Find a transfer queue family that specifically doesn't support graphics
            if(queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT && !(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
                indices.transferFamily = i;
            }

            if(indices.isComplete()) {
                break;
            }

            ++i;
        }

        return indices;
    }

    static bool checkDeviceExtensionsSupport(VkPhysicalDevice device, std::vector<char const *> const &extensions) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        for(char const *extensionName : extensions) {
            bool found = false;

            for(auto const &extensionProperties : availableExtensions) {
                if(std::strcmp(extensionName, extensionProperties.extensionName) == 0) {
                    found = true;
                    break;
                }
            }

            if(!found) {
                return false;
            }
        }

        return true;
    }

    static SwapchainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
        SwapchainSupportDetails details;

        //Surface capabilities
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        //Surface formats
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        if(formatCount > 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        //Surface presentation modes
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        if(presentModeCount > 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    static bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<char const *> const &extensions) {
        //TODO: It might be better to give each physical device a score in future, so we can try and get the best one but fall back to others if not

        //Feature (texture compression, 64 bit floats, multi viewport rendering)
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        QueueFamilyIndices indices        = findQueueFamilies(device, surface);
        bool const extentionsAreSupported = checkDeviceExtensionsSupport(device, extensions);

        bool swapChainIsAdequate = false;
        if(extentionsAreSupported) {
            SwapchainSupportDetails swapChainSupport = querySwapChainSupport(device, surface);
            //We'll consider the swap chain adequate if we have once supported image format and presentation mode
            swapChainIsAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return indices.isComplete() && extentionsAreSupported && swapChainIsAdequate && deviceFeatures.samplerAnisotropy;
    }

    VKGraphicsDevice::VKGraphicsDevice(void *nativeWindow) {
        std::vector<char const *> deviceExtensions{
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };

        std::vector<char const *> requiredExtensions {
            VK_KHR_SURFACE_EXTENSION_NAME,
                "VK_KHR_win32_surface",//TODO: Platform agnostic extensions
#if GARLIC_DEBUG
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
        };

#if GARLIC_DEBUG
        std::vector<char const *> const validationLayers{
            "VK_LAYER_KHRONOS_validation"
        };

        if(!checkValidationLayerSupport(validationLayers)) {
            GARLIC_LOG(garlicLogContext, garlic::LogLevel::Warning, "Vulkan validation layers are not supported on this device. Unable to provide debugging infomation");
        }
#endif

        //CREATE INSTANCE
        VkInstance instance{ VK_NULL_HANDLE };
        VkDebugUtilsMessengerEXT debugMessenger{ VK_NULL_HANDLE };
        {
            VkApplicationInfo appInfo{
                .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pApplicationName   = "No Name",               //TODO: Get app name
                .applicationVersion = VK_MAKE_VERSION(1, 0, 0),//TODO: Get app version
                .pEngineName        = "Garlic",                //TODO: Add a variable for the engine name
                .engineVersion      = VK_MAKE_VERSION(1, 0, 0),//TODO: Get engine version
                .apiVersion         = VK_API_VERSION_1_2,
            };

#if GARLIC_DEBUG
            VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo{
                .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                .messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                .pfnUserCallback = debugCallback,
                .pUserData       = nullptr,
            };
#endif

            VkInstanceCreateInfo createInfo {
                .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
#if GARLIC_DEBUG
                .pNext                = &debugMessengerCreateInfo,//Setting the pNext allows us to debug the creation and destruction of the instance (as normaly we need an instance pointer to enable debugging)
                    .pApplicationInfo = &appInfo,
                .enabledLayerCount    = static_cast<uint32_t>(std::size(validationLayers)),
                .ppEnabledLayerNames  = std::data(validationLayers),
#else
                .pNext               = nullptr,
                .pApplicationInfo    = &appInfo,
                .enabledLayerCount   = 0,
                .ppEnabledLayerNames = nullptr,
#endif
                .enabledExtensionCount   = static_cast<uint32_t>(std::size(requiredExtensions)),
                .ppEnabledExtensionNames = std::data(requiredExtensions),
            };

            if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
                GARLIC_LOG(garlicLogContext, garlic::LogLevel::Error, "Failed to create VK instance");
                return;
            }

#if GARLIC_DEBUG
            //TODO: Move this debug messenger setup else where
            if(createDebugUtilsMessengerEXT(instance, &debugMessengerCreateInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
                GARLIC_LOG(garlicLogContext, garlic::LogLevel::Error, "Failed to create vk debug message callback");
                return;
            }
#endif
        }

        //CREATE SURFACE
        VkSurfaceKHR surface;
        {
            //TODO: Platform agnostic surface creation
            VkWin32SurfaceCreateInfoKHR createInfo{
                .sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
                .hinstance = GetModuleHandle(nullptr),
                .hwnd      = reinterpret_cast<HWND>(nativeWindow),
            };

            if(vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
                GARLIC_LOG(garlicLogContext, garlic::LogLevel::Error, "Failed to create Vulkan surface");
                return;
            }
        }

        //PICK PHYSICAL DEVICE
        VkPhysicalDevice physicalDevice;
        {
            uint32_t deviceCount = 0;
            vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
            if(deviceCount == 0) {
                GARLIC_LOG(garlicLogContext, garlic::LogLevel::Error, "failed to find GPUs with Vulkan support!");
                return;
            }
            std::vector<VkPhysicalDevice> devices(deviceCount);
            vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

            for(auto const &device : devices) {
                if(isDeviceSuitable(device, surface, deviceExtensions)) {
                    physicalDevice = device;
                    break;
                }
            }

            if(physicalDevice == VK_NULL_HANDLE) {
                GARLIC_LOG(garlicLogContext, garlic::LogLevel::Error, "failed to find a suitable GPU!");
                return;
            } else {
                VkPhysicalDeviceProperties devicePoperties;
                vkGetPhysicalDeviceProperties(physicalDevice, &devicePoperties);

                GARLIC_LOG(garlicLogContext, garlic::LogLevel::Info, "Vulkan capable physical device found");
                GARLIC_LOG(garlicLogContext, garlic::LogLevel::Info, "\tDevice:\t{0}", devicePoperties.deviceName);
                GARLIC_LOG(garlicLogContext, garlic::LogLevel::Info, "\tDriver:\t{0}.{1}.{2}", VK_VERSION_MAJOR(devicePoperties.driverVersion), VK_VERSION_MINOR(devicePoperties.driverVersion), VK_VERSION_PATCH(devicePoperties.driverVersion));
                GARLIC_LOG(garlicLogContext, garlic::LogLevel::Info, "\tAPI:\t{0}.{1}.{2}", VK_VERSION_MAJOR(devicePoperties.apiVersion), VK_VERSION_MINOR(devicePoperties.apiVersion), VK_VERSION_PATCH(devicePoperties.apiVersion));
            }
        }

        QueueFamilyIndices queueFamilyIndices;

        //CREATE LOGICAL DEVICE
        VkDevice logicalDevice;
        {
            queueFamilyIndices = findQueueFamilies(physicalDevice, surface);

            std::set<uint32_t> uniqueQueueFamilies{
                *queueFamilyIndices.graphicsFamily,
                *queueFamilyIndices.presentFamily,
                *queueFamilyIndices.transferFamily
            };

            float constexpr queuePriority = 1.0f;
            std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
            for(uint32_t queueFamily : uniqueQueueFamilies) {
                VkDeviceQueueCreateInfo queueCreateinfo{
                    .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    .queueFamilyIndex = queueFamily,
                    .queueCount       = 1,
                    .pQueuePriorities = &queuePriority,
                };
                queueCreateInfos.push_back(queueCreateinfo);
            }

            //Sepcify our device features
            VkPhysicalDeviceFeatures deviceFeatures{
                .samplerAnisotropy = VK_TRUE,//TODO: Maybe we want to expose this to users?
            };

            VkDeviceCreateInfo createInfo {
                .sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                .queueCreateInfoCount = static_cast<uint32_t>(std::size(queueCreateInfos)),
                .pQueueCreateInfos    = std::data(queueCreateInfos),

#if GARLIC_DEBUG
                //We don't need to do this as device specific validation layers are no more. But seeing as it's the same data we can reuse them to support older versions
                    .enabledLayerCount = static_cast<uint32_t>(validationLayers.size()),
                .ppEnabledLayerNames   = validationLayers.data(),
#else
                .enabledLayerCount   = 0,
#endif
                .enabledExtensionCount   = static_cast<uint32_t>(std::size(deviceExtensions)),
                .ppEnabledExtensionNames = std::data(deviceExtensions),
                .pEnabledFeatures        = &deviceFeatures,
            };

            if(vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
                GARLIC_LOG(garlicLogContext, garlic::LogLevel::Error, "Failed to create logical device");
                return;
            }
        }

        devicePtr = DevicePointer(instance, surface, physicalDevice, logicalDevice, debugMessenger);
        factory   = std::make_shared<VKGraphicsFactory>(devicePtr, std::move(queueFamilyIndices), querySwapChainSupport(physicalDevice, surface));
    }

    VKGraphicsDevice::VKGraphicsDevice(VKGraphicsDevice &&other) noexcept = default;

    VKGraphicsDevice &VKGraphicsDevice::operator=(VKGraphicsDevice &&other) noexcept = default;

    VKGraphicsDevice::~VKGraphicsDevice() = default;

    std::shared_ptr<GraphicsFactory> VKGraphicsDevice::getGraphicsFactory() const {
        return factory;
    }

    void VKGraphicsDevice::waitForIdleDevice() {
        vkDeviceWaitIdle(devicePtr.get());
    }

    GraphicsDevice::Limits VKGraphicsDevice::getLimits() const {
        VkPhysicalDeviceProperties devicePoperties;
        vkGetPhysicalDeviceProperties(devicePtr.getPhysical(), &devicePoperties);

        return {
            devicePoperties.limits.minUniformBufferOffsetAlignment
        };
    }
}