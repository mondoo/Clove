#if CLOVE_PLATFORM_WINDOWS
    #define VK_USE_PLATFORM_WIN32_KHR
#elif CLOVE_PLATFORM_MACOS
#elif CLOVE_PLATFORM_LINUX
    #define VK_USE_PLATFORM_XLIB_KHR
#endif

//Vulkan needs to be before everything otherwise header guards from other headers will prevent compilation
#include <vulkan/vk_sdk_platform.h>
#include <vulkan/vulkan.h>
//~~~~

//Remove xlib defs from vulkan.h
#if CLOVE_PLATFORM_LINUX
    #undef None
    #undef Success
#endif

#include "Clove/Graphics/Vulkan/VulkanDevice.hpp"
#include "Clove/Graphics/Vulkan/VulkanFactory.hpp"

#include <Clove/Definitions.hpp>
#include <Clove/Log/Log.hpp>
#include <set>

CLOVE_DECLARE_LOG_CATEGORY(VULKAN)

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messagType,
    VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData,
    void *pUserData) {
    if((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) != 0) {
        CLOVE_LOG(LOG_CATEGORY_VULKAN, garlic::clove::LogLevel::Trace, pCallbackData->pMessage);
    } else if((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) != 0) {
        CLOVE_LOG(LOG_CATEGORY_VULKAN, garlic::clove::LogLevel::Warning, pCallbackData->pMessage);
    } else if((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) != 0) {
        CLOVE_LOG(LOG_CATEGORY_VULKAN, garlic::clove::LogLevel::Error, pCallbackData->pMessage);
    }

    return VK_FALSE;
}

namespace garlic::clove {
    namespace {
        VkResult createDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT const *pCreateInfo, VkAllocationCallbacks const *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger) {
            auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
            if(func != nullptr) {
                return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
            } else {
                return VK_ERROR_EXTENSION_NOT_PRESENT;
            }
        }

        bool checkValidationLayerSupport(std::vector<char const *> const &validationLayers) {
            uint32_t layerCount{ 0 };
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

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
            QueueFamilyIndices indices;

            uint32_t queueFamilyCount{ 0 };
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

            for(int i{ 0 }; auto const &queueFamily : queueFamilies) {
                //Make sure we have the queue family that'll let us render graphics
                if((queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0u) {
                    indices.graphicsFamily = i;
                }

                //If we have a surface, check if we have presentation support. Otherwise we're running headless
                if(surface != VK_NULL_HANDLE) {
                    VkBool32 presentSupport{ VK_FALSE };
                    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
                    if(presentSupport == VK_TRUE) {
                        indices.presentFamily = i;
                    }
                }

                //Find a transfer queue family that specifically doesn't support graphics
                if(((queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) != 0u) && ((queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0u)) {
                    indices.transferFamily = i;
                }

                //Find a queue family for compute operations
                if(queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT){
                    indices.computeFamily = i;
                }

                bool const requirePresentFamily{ surface != VK_NULL_HANDLE };
                if(indices.isComplete(requirePresentFamily)) {
                    break;
                }

                ++i;
            }

            return indices;
        }

        bool checkDeviceExtensionsSupport(VkPhysicalDevice device, std::vector<char const *> const &extensions) {
            uint32_t extensionCount{ 0 };
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

        bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<char const *> const &extensions) {
            VkPhysicalDeviceFeatures deviceFeatures;
            vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

            QueueFamilyIndices indices{ findQueueFamilies(device, surface) };
            bool const extentionsAreSupported{ checkDeviceExtensionsSupport(device, extensions) };

            std::optional<bool> surfaceIsAdequate{};
            if(extentionsAreSupported) {
                //If we don't have a surface then we're headless
                if(surface != VK_NULL_HANDLE) {
                    SurfaceSupportDetails const surfaceSupport{ SurfaceSupportDetails::query(device, surface) };
                    //We'll consider the surface adequate if we have one supported image format and presentation mode
                    surfaceIsAdequate = !surfaceSupport.formats.empty() && !surfaceSupport.presentModes.empty();
                }
            } else {
                surfaceIsAdequate = false;
            }

            bool const requirePresentFamily{ surface != VK_NULL_HANDLE };
            return indices.isComplete(requirePresentFamily) && extentionsAreSupported && surfaceIsAdequate.value_or(true) && (deviceFeatures.samplerAnisotropy != 0u);
        }
    }

    VulkanDevice::VulkanDevice(std::any nativeWindow) {
        std::vector<char const *> deviceExtensions{
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };

        std::vector<char const *> requiredExtensions {
            VK_KHR_SURFACE_EXTENSION_NAME,
#if CLOVE_PLATFORM_WINDOWS
                VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif CLOVE_PLATFORM_MACOS
#elif CLOVE_PLATFORM_LINUX
                VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
#endif
#if CLOVE_DEBUG
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
        };

#if CLOVE_DEBUG
        std::vector<char const *> const validationLayers{
            "VK_LAYER_KHRONOS_validation"
        };

        if(!checkValidationLayerSupport(validationLayers)) {
            CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Warning, "Vulkan validation layers are not supported on this device. Unable to provide debugging infomation");
        }
#endif

        //Create instance
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

#if CLOVE_DEBUG
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
#if CLOVE_DEBUG
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
                CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to create VK instance");
                return;
            }

#if CLOVE_DEBUG
            if(createDebugUtilsMessengerEXT(instance, &debugMessengerCreateInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
                CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to create vk debug message callback");
                return;
            }
#endif
        }

        //Create surface
        VkSurfaceKHR surface{ VK_NULL_HANDLE };
        if(nativeWindow.has_value()) {
#if CLOVE_PLATFORM_WINDOWS
            VkWin32SurfaceCreateInfoKHR createInfo{
                .sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
                .hinstance = GetModuleHandle(nullptr),
                .hwnd      = std::any_cast<HWND>(nativeWindow),
            };

            if(vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
                CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to create Vulkan surface");
                return;
            }
#elif CLOVE_PLATFORM_MACOS
            CLOVE_ASSERT(false, "Vulkan implementation not provided on MacOS");
#elif CLOVE_PLATFORM_LINUX
            auto const [display, window] = std::any_cast<std::pair<Display *, ::Window>>(nativeWindow);

            VkXlibSurfaceCreateInfoKHR createInfo{
                .sType  = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
                .dpy    = display,
                .window = window,
            };

            if(vkCreateXlibSurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
                CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to create Vulkan surface");
                return;
            }
#endif
        }

        //Pick physical device
        VkPhysicalDevice physicalDevice{ nullptr };
        {
            uint32_t deviceCount = 0;
            vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
            if(deviceCount == 0) {
                CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "failed to find GPUs with Vulkan support!");
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
                CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "failed to find a suitable GPU!");
                return;
            } else {
                VkPhysicalDeviceProperties devicePoperties;
                vkGetPhysicalDeviceProperties(physicalDevice, &devicePoperties);

                CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Info, "Vulkan capable physical device found");
                CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Info, "\tDevice:\t{0}", devicePoperties.deviceName);
                CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Info, "\tDriver:\t{0}.{1}.{2}", VK_VERSION_MAJOR(devicePoperties.driverVersion), VK_VERSION_MINOR(devicePoperties.driverVersion), VK_VERSION_PATCH(devicePoperties.driverVersion));
                CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Info, "\tAPI:\t{0}.{1}.{2}", VK_VERSION_MAJOR(devicePoperties.apiVersion), VK_VERSION_MINOR(devicePoperties.apiVersion), VK_VERSION_PATCH(devicePoperties.apiVersion));
            }
        }

        QueueFamilyIndices queueFamilyIndices{ findQueueFamilies(physicalDevice, surface) };

        //Create logical device
        VkDevice logicalDevice{ nullptr };
        {
            std::set<uint32_t> uniqueQueueFamilies{
                *queueFamilyIndices.graphicsFamily,
                *queueFamilyIndices.transferFamily
            };
            if(queueFamilyIndices.presentFamily.has_value()) {
                uniqueQueueFamilies.emplace(*queueFamilyIndices.presentFamily);
            }

            float constexpr queuePriority{ 1.0f };
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

#if CLOVE_DEBUG
                //We don't need to do this as device specific validation layers are no more. But seeing as it's the same data we can reuse them to support older versions
                    .enabledLayerCount = static_cast<uint32_t>(validationLayers.size()),
                .ppEnabledLayerNames   = validationLayers.data(),
#else
                .enabledLayerCount = 0,
#endif
                .enabledExtensionCount   = static_cast<uint32_t>(std::size(deviceExtensions)),
                .ppEnabledExtensionNames = std::data(deviceExtensions),
                .pEnabledFeatures        = &deviceFeatures,
            };

            if(vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
                CLOVE_LOG(LOG_CATEGORY_CLOVE, LogLevel::Error, "Failed to create logical device");
                return;
            }
        }

        devicePtr = DevicePointer{ instance, surface, physicalDevice, logicalDevice, debugMessenger };
        factory   = std::make_shared<VulkanFactory>(devicePtr, queueFamilyIndices);
    }

    VulkanDevice::VulkanDevice(VulkanDevice &&other) noexcept = default;

    VulkanDevice &VulkanDevice::operator=(VulkanDevice &&other) noexcept = default;

    VulkanDevice::~VulkanDevice() = default;

    std::shared_ptr<GhaFactory> VulkanDevice::getGraphicsFactory() const {
        return factory;
    }

    void VulkanDevice::waitForIdleDevice() {
        vkDeviceWaitIdle(devicePtr.get());
    }

    GhaDevice::Limits VulkanDevice::getLimits() const {
        VkPhysicalDeviceProperties devicePoperties;
        vkGetPhysicalDeviceProperties(devicePtr.getPhysical(), &devicePoperties);

        return {
            devicePoperties.limits.minUniformBufferOffsetAlignment
        };
    }
}