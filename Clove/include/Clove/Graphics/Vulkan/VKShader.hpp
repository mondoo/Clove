#pragma once

//TODO: Remove
#include <vulkan/vulkan.h>

namespace clv::gfx::vk {
	class VKShader {
		//VARIABLES
	private:
		VkDevice device;

		VkShaderModule module = VK_NULL_HANDLE;

		//FUNCTIONS
	public:
		//TODO: Ctors
		VKShader(VkDevice device, std::string_view filePath);
		VKShader(VkDevice device, std::vector<std::byte> byteCode);

		~VKShader();

		VkShaderModule getModule() const;
	};
}