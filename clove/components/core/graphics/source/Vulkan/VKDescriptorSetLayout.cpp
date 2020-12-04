#include "Clove/Graphics/Vulkan/VKDescriptorSetLayout.hpp"

namespace garlic::clove {
    VKDescriptorSetLayout::VKDescriptorSetLayout(DevicePointer device, VkDescriptorSetLayout layout, Descriptor descriptor)
        : device{ std::move(device) }
        , layout{ layout }
        , descriptor{ std::move(descriptor) } {
    }

    VKDescriptorSetLayout::VKDescriptorSetLayout(VKDescriptorSetLayout &&other) noexcept = default;

    VKDescriptorSetLayout &VKDescriptorSetLayout::operator=(VKDescriptorSetLayout &&other) noexcept = default;

    VKDescriptorSetLayout::~VKDescriptorSetLayout() {
        vkDestroyDescriptorSetLayout(device.get(), layout, nullptr);
    }
}