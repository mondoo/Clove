#include "Clove/Graphics/Vulkan/VKGraphicsFactory.hpp"

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

namespace clv::gfx::vk{
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

	VKGraphicsFactory::VKGraphicsFactory(void* nativeWindow) {
		std::vector<const char*> deviceExtensions {
			VK_KHR_SURFACE_EXTENSION_NAME,
		};

		std::vector<const char*> requiredExtensions {
			"VK_KHR_win32_surface", //TODO: Platform agnostic extensions
#if CLV_DEBUG
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
		};

#if CLV_DEBUG
		const std::vector<const char*> validationLayers{
			"VK_LAYER_KHRONOS_validation"
		};

		if(!checkValidationLayerSupport(validationLayers)) {
			CLV_LOG_WARN("Vulkan validation layers are not supported on this device!");
		}
#endif

		VkApplicationInfo appInfo{};
		appInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName	= "No Name";				//TODO: Get app name
		appInfo.applicationVersion	= VK_MAKE_VERSION(1, 0, 0); //TODO: Get app version
		appInfo.pEngineName			= "Garlic";					//TODO: Add a variable for the engine name
		appInfo.engineVersion		= VK_MAKE_VERSION(1, 0, 0); //TODO: Get engine version
		appInfo.apiVersion			= VK_API_VERSION_1_2;

#if CLV_DEBUG
		VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo{};
		debugMessengerCreateInfo.sType				= VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugMessengerCreateInfo.messageSeverity	= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugMessengerCreateInfo.messageType		= VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugMessengerCreateInfo.pfnUserCallback	= debugCallback;
		debugMessengerCreateInfo.pUserData			= nullptr;
#endif

		VkInstanceCreateInfo createInfo{};
		createInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo			= &appInfo;
		createInfo.enabledExtensionCount	= std::size(requiredExtensions);
		createInfo.ppEnabledExtensionNames	= std::data(requiredExtensions);
#if CLV_DEBUG
		createInfo.enabledLayerCount		= std::size(validationLayers);
		createInfo.ppEnabledLayerNames		= std::data(validationLayers);
		createInfo.pNext					= &debugMessengerCreateInfo; //Setting the pNext allows us to debug the creation and destruction of the instance (as normaly we need an instance pointer to enable debugging)
#else
		createInfo.enabledLayerCount		= 0;
		createInfo.ppEnabledLayerNames		= nullptr;
		createInfo.pNext					= nullptr;
#endif

		if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			CLV_LOG_ERROR("Failed to create VK instance");
			return;
		}

#if CLV_DEBUG
		//TODO: Move this debug messenger setup else where
		if(createDebugUtilsMessengerEXT(instance, &debugMessengerCreateInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
			CLV_LOG_ERROR("Failed to create vk debug message callback");
		}
#endif

		//TODO: Platform agnostic surface creation
		VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{};
		surfaceCreateInfo.sType		= VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
		surfaceCreateInfo.hwnd		= reinterpret_cast<HWND>(nativeWindow);

		if(vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface) != VK_SUCCESS) {
			CLV_LOG_ERROR("Failed to create Vulkan surface");
		}
	}

	VKGraphicsFactory::~VKGraphicsFactory() {
#if CLV_DEBUG
		destroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
#endif
		vkDestroySurfaceKHR(instance, surface, nullptr);
		
		vkDestroyInstance(instance, nullptr);
	}
}