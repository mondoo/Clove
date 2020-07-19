#pragma once

#include <vulkan/vulkan.h>

//TODO: Move out generic types
namespace clv::gfx {
	enum class ImageFormat {
		Unkown,
		B8G8R8A8_SRGB,
		B8G8R8A8_UNORM
	};

	//TODO: Have Success and Error then OR those with other codes so you can just check for succes or error
	enum class Result {
		Success,
		Success_SwapchainSuboptimal,
		
		Error_SwapchainOutOfDate,

		Unkown
	};

	enum class VertexAttributeFormat{
		R32_SFLOAT,
		R32G32_SFLOAT,
        R32G32B32_SFLOAT,
        R32G32B32A32_SFLOAT
	};

	struct VertexInputBindingDescriptor {
        uint32_t binding = 0;
        uint32_t stride = 0;
    };

	struct VertexAttributeDescriptor{
        uint32_t binding  = 0;
		uint32_t location = 0;
        VertexAttributeFormat format;
        uint32_t offset = 0;
	};

	enum class DescriptorType {
        UniformBuffer,
    };

	enum class SharingMode {
        Exclusive,
        Concurrent
    };

	enum class MemoryProperties {
        DeviceLocal,//GPU optimised, can't be written to be CPU
        HostVisible,//Can be written to by CPU, not GPU optimised
    };
}

namespace clv::gfx::vk {
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		std::optional<uint32_t> transferFamily;

		bool isComplete() const {
			return graphicsFamily && presentFamily && transferFamily;
		}
	};

	struct SwapchainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};
}