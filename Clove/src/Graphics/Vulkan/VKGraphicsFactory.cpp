#include "Clove/Graphics/Vulkan/VKGraphicsFactory.hpp"

#include "Clove/GRaphics/Vulkan/VKImage.hpp"
#include "Clove/Graphics/Vulkan/VKBuffer.hpp"
#include "Clove/Graphics/Vulkan/VKFence.hpp"
#include "Clove/Graphics/Vulkan/VKFramebuffer.hpp"
#include "Clove/Graphics/Vulkan/VKRenderPass.hpp"
#include "Clove/Graphics/Vulkan/VKSemaphore.hpp"
#include "Clove/Graphics/Vulkan/VKShader.hpp"

//TODO: Abstract away
#include "Clove/Platform/Windows/CloveWindows.hpp"

#include <vulkan/vulkan_win32.h>

//TODO: Move this callback (and the set up) into VKException.hpp
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messagType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

namespace clv::gfx::vk {
    static VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if(func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    static void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if(func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    static bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers) {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for(const char* layerName : validationLayers) {
            bool layerFound = false;

            for(const auto& layerProperties : availableLayers) {
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
        for(const auto& queueFamily : queueFamilies) {
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

    static bool checkDeviceExtensionsSupport(VkPhysicalDevice device, const std::vector<const char*>& extensions) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        for(const char* extensionName : extensions) {
            bool found = false;

            for(const auto& extensionProperties : availableExtensions) {
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

    static bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const std::vector<const char*>& extensions) {
        //TODO: It might be better to give each physical device a score in future, so we can try and get the best one but fall back to others if not

        //Basic properties (name, type, supported vk version)
        //VkPhysicalDeviceProperties devicePoperties;
        //vkGetPhysicalDeviceProperties(device, &devicePoperties);

        //Feature (texture compression, 64 bit floats, multi viewport rendering)
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        QueueFamilyIndices indices        = findQueueFamilies(device, surface);
        const bool extentionsAreSupported = checkDeviceExtensionsSupport(device, extensions);

        bool swapChainIsAdequate = false;
        if(extentionsAreSupported) {
            SwapchainSupportDetails swapChainSupport = querySwapChainSupport(device, surface);
            //We'll consider the swap chain adequate if we have once supported image format and presentation mode
            swapChainIsAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return indices.isComplete() && extentionsAreSupported && swapChainIsAdequate && deviceFeatures.samplerAnisotropy;
    }

    VKGraphicsFactory::VKGraphicsFactory(void* nativeWindow) {
        std::vector<const char*> deviceExtensions{
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };

        std::vector<const char*> requiredExtensions {
            VK_KHR_SURFACE_EXTENSION_NAME,
                "VK_KHR_win32_surface",//TODO: Platform agnostic extensions
#if GARLIC_DEBUG
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
        };

#if GARLIC_DEBUG
        const std::vector<const char*> validationLayers{
            "VK_LAYER_KHRONOS_validation"
        };

        if(!checkValidationLayerSupport(validationLayers)) {
            GARLIC_LOG(garlicLogContext, Log::Level::Warning, "Vulkan validation layers are not supported on this device. Unable to provide debugging infomation");
        }
#endif

        //CREATE INSTANCE
        {
            VkApplicationInfo appInfo{};
            appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName   = "No Name";               //TODO: Get app name
            appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);//TODO: Get app version
            appInfo.pEngineName        = "Garlic";                //TODO: Add a variable for the engine name
            appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);//TODO: Get engine version
            appInfo.apiVersion         = VK_API_VERSION_1_2;

#if GARLIC_DEBUG
            VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo{};
            debugMessengerCreateInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            debugMessengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            debugMessengerCreateInfo.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            debugMessengerCreateInfo.pfnUserCallback = debugCallback;
            debugMessengerCreateInfo.pUserData       = nullptr;
#endif

            VkInstanceCreateInfo createInfo{};
            createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo        = &appInfo;
            createInfo.enabledExtensionCount   = std::size(requiredExtensions);
            createInfo.ppEnabledExtensionNames = std::data(requiredExtensions);
#if GARLIC_DEBUG
            createInfo.enabledLayerCount   = std::size(validationLayers);
            createInfo.ppEnabledLayerNames = std::data(validationLayers);
            createInfo.pNext               = &debugMessengerCreateInfo;//Setting the pNext allows us to debug the creation and destruction of the instance (as normaly we need an instance pointer to enable debugging)
#else
            createInfo.enabledLayerCount   = 0;
            createInfo.ppEnabledLayerNames = nullptr;
            createInfo.pNext               = nullptr;
#endif

            if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
                GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create VK instance");
                return;
            }

#if GARLIC_DEBUG
            //TODO: Move this debug messenger setup else where
            if(createDebugUtilsMessengerEXT(instance, &debugMessengerCreateInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
                GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create vk debug message callback");
                return;
            }
#endif
        }

        //CREATE SURFACE
        {
            //TODO: Platform agnostic surface creation
            VkWin32SurfaceCreateInfoKHR createInfo{};
            createInfo.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
            createInfo.hinstance = GetModuleHandle(nullptr);
            createInfo.hwnd      = reinterpret_cast<HWND>(nativeWindow);

            if(vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
                GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create Vulkan surface");
                return;
            }
        }

        //PICK PHYSICAL DEVICE
        {
            uint32_t deviceCount = 0;
            vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
            if(deviceCount == 0) {
                GARLIC_LOG(garlicLogContext, Log::Level::Error, "failed to find GPUs with Vulkan support!");
                return;
            }
            std::vector<VkPhysicalDevice> devices(deviceCount);
            vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

            for(const auto& device : devices) {
                if(isDeviceSuitable(device, surface, deviceExtensions)) {
                    physicalDevice = device;
                    break;
                }
            }

            if(physicalDevice == VK_NULL_HANDLE) {
                GARLIC_LOG(garlicLogContext, Log::Level::Error, "failed to find a suitable GPU!");
                return;
            } else {
                VkPhysicalDeviceProperties devicePoperties;
                vkGetPhysicalDeviceProperties(physicalDevice, &devicePoperties);

                GARLIC_LOG(garlicLogContext, Log::Level::Info, "Vulkan capable physical device found");
                GARLIC_LOG(garlicLogContext, Log::Level::Info, "\tDevice:\t{0}", devicePoperties.deviceName);
                GARLIC_LOG(garlicLogContext, Log::Level::Info, "\tDriver:\t{0}.{1}.{2}", VK_VERSION_MAJOR(devicePoperties.driverVersion), VK_VERSION_MINOR(devicePoperties.driverVersion), VK_VERSION_PATCH(devicePoperties.driverVersion));
                GARLIC_LOG(garlicLogContext, Log::Level::Info, "\tAPI:\t{0}.{1}.{2}", VK_VERSION_MAJOR(devicePoperties.apiVersion), VK_VERSION_MINOR(devicePoperties.apiVersion), VK_VERSION_PATCH(devicePoperties.apiVersion));
            }
        }

        //CREATE LOGICAL DEVICE
        {
            queueFamilyIndices = findQueueFamilies(physicalDevice, surface);

            std::set<uint32_t> uniqueQueueFamilies{
                *queueFamilyIndices.graphicsFamily,
                *queueFamilyIndices.presentFamily,
                *queueFamilyIndices.transferFamily
            };

            constexpr float queuePriority = 1.0f;
            std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
            for(uint32_t queueFamily : uniqueQueueFamilies) {
                VkDeviceQueueCreateInfo queueCreateinfo{};
                queueCreateinfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueCreateinfo.queueFamilyIndex = queueFamily;
                queueCreateinfo.queueCount       = 1;
                queueCreateinfo.pQueuePriorities = &queuePriority;

                queueCreateInfos.push_back(queueCreateinfo);
            }

            //Sepcify our device features
            VkPhysicalDeviceFeatures deviceFeatures{};
            deviceFeatures.samplerAnisotropy = VK_TRUE;//TODO: Maybe we want to expose this to users?

            VkDeviceCreateInfo createInfo{};
            createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            createInfo.queueCreateInfoCount    = static_cast<uint32_t>(std::size(queueCreateInfos));
            createInfo.pQueueCreateInfos       = std::data(queueCreateInfos);
            createInfo.pEnabledFeatures        = &deviceFeatures;
            createInfo.enabledExtensionCount   = static_cast<uint32_t>(std::size(deviceExtensions));
            createInfo.ppEnabledExtensionNames = std::data(deviceExtensions);
#if GARLIC_DEBUG
            //We don't need to do this as device specific validation layers are no more. But seeing as it's the same data we can reuse them to support older versions
            createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
#else
            createInfo.enabledLayerCount   = 0;
#endif

            if(vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
                GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create logical device");
                return;
            }
        }
    }

    VKGraphicsFactory::~VKGraphicsFactory() {
#if GARLIC_DEBUG
        destroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
#endif
        vkDestroyDevice(logicalDevice, nullptr);
        vkDestroySurfaceKHR(instance, surface, nullptr);

        vkDestroyInstance(instance, nullptr);
    }

    std::unique_ptr<VKGraphicsQueue> VKGraphicsFactory::createGraphicsQueue(CommandQueueDescriptor descriptor) {
        return std::make_unique<VKGraphicsQueue>(logicalDevice, *queueFamilyIndices.graphicsFamily, std::move(descriptor));
    }

    std::unique_ptr<VKPresentQueue> VKGraphicsFactory::createPresentQueue() {
        return std::make_unique<VKPresentQueue>(logicalDevice, *queueFamilyIndices.presentFamily);
    }

    std::unique_ptr<VKTransferQueue> VKGraphicsFactory::createTransferQueue(CommandQueueDescriptor descriptor) {
        return std::make_unique<VKTransferQueue>(logicalDevice, *queueFamilyIndices.transferFamily, std::move(descriptor));
    }

    std::unique_ptr<Swapchain> VKGraphicsFactory::createSwapChain(Swapchain::Descriptor descriptor) {
        return std::make_unique<VKSwapchain>(logicalDevice, querySwapChainSupport(physicalDevice, surface), surface, queueFamilyIndices, std::move(descriptor));
    }

    std::unique_ptr<Shader> VKGraphicsFactory::createShader(std::string_view filePath) {
        return std::make_unique<VKShader>(logicalDevice, filePath);
    }

    std::unique_ptr<Shader> VKGraphicsFactory::createShader(std::vector<std::byte> byteCode) {
        return std::make_unique<VKShader>(logicalDevice, std::move(byteCode));
    }

    std::unique_ptr<RenderPass> VKGraphicsFactory::createRenderPass(RenderPass::Descriptor descriptor) {
        return std::make_unique<VKRenderPass>(logicalDevice, std::move(descriptor));
    }

    std::unique_ptr<VKDescriptorSetLayout> VKGraphicsFactory::createDescriptorSetLayout(DescriptorSetLayoutDescriptor descriptor) {
        return std::make_unique<VKDescriptorSetLayout>(logicalDevice, std::move(descriptor));
    }

    std::unique_ptr<VKPipelineObject> VKGraphicsFactory::createPipelineObject(PiplineObjectDescriptor descriptor) {
        return std::make_unique<VKPipelineObject>(logicalDevice, std::move(descriptor));
    }

    std::unique_ptr<Framebuffer> VKGraphicsFactory::createFramebuffer(Framebuffer::Descriptor descriptor) {
        return std::make_unique<VKFramebuffer>(logicalDevice, std::move(descriptor));
    }

    std::unique_ptr<VKDescriptorPool> VKGraphicsFactory::createDescriptorPool(DescriptorPoolDescriptor descriptor) {
        return std::make_unique<VKDescriptorPool>(logicalDevice, std::move(descriptor));
    }

    std::unique_ptr<Semaphore> VKGraphicsFactory::createSemaphore() {
        return std::make_unique<VKSemaphore>(logicalDevice);
    }

    std::unique_ptr<Fence> VKGraphicsFactory::createFence(Fence::Descriptor descriptor) {
        return std::make_unique<VKFence>(logicalDevice, std::move(descriptor));
    }

    std::unique_ptr<GraphicsBuffer> VKGraphicsFactory::createBuffer(GraphicsBuffer::Descriptor descriptor) {
        return std::make_unique<VKBuffer>(logicalDevice, physicalDevice, std::move(descriptor), queueFamilyIndices);
    }

    std::unique_ptr<GraphicsImage> VKGraphicsFactory::createImage(GraphicsImage::Descriptor descriptor) {
        return std::make_unique<VKImage>(logicalDevice, physicalDevice, std::move(descriptor), queueFamilyIndices);
    }

    std::unique_ptr<VKSampler> VKGraphicsFactory::createSampler(SamplerDescriptor descriptor) {
        return std::make_unique<VKSampler>(logicalDevice, std::move(descriptor));
    }

    void VKGraphicsFactory::waitForIdleDevice() {
        vkDeviceWaitIdle(logicalDevice);
    }
}