#pragma once

#include <vulkan/vulkan.h>

namespace clv::gfx::vk{
	class VKGraphicsFactory{
		//VARIABLES
	private:
		VkDebugUtilsMessengerEXT debugMessenger;

		VkInstance instance;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKGraphicsFactory();
		~VKGraphicsFactory();
	};
}