#include "Clove/Graphics/Vulkan/VKDescriptorSetLayout.hpp"

#include "Clove/Graphics/Vulkan/VKDescriptor.hpp"
#include "Clove/Graphics/Vulkan/VKShader.hpp"
#include "Clove/Log.hpp"

#include <Root/Definitions.hpp>

namespace clv::gfx::vk {
    VKDescriptorSetLayout::VKDescriptorSetLayout(DevicePointer device, Descriptor descriptor)
        : device(std::move(device))
        , descriptor(std::move(descriptor)) {
        std::vector<VkDescriptorSetLayoutBinding> layoutBindings(std::size(this->descriptor.bindings));

        for(size_t i = 0; i < std::size(layoutBindings); ++i) {
            const DescriptorSetBindingInfo& bindingDescriptor = this->descriptor.bindings[i];

            layoutBindings[i].binding            = bindingDescriptor.binding;
            layoutBindings[i].descriptorType     = getDescriptorType(bindingDescriptor.type);
            layoutBindings[i].descriptorCount    = bindingDescriptor.arraySize;
            layoutBindings[i].stageFlags         = VKShader::convertStage(bindingDescriptor.stage);
            layoutBindings[i].pImmutableSamplers = nullptr;
        }

        VkDescriptorSetLayoutCreateInfo createInfo{
            .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext        = nullptr,
            .flags        = 0,
            .bindingCount = static_cast<uint32_t>(std::size(layoutBindings)),
            .pBindings    = std::data(layoutBindings),
        };

        if(vkCreateDescriptorSetLayout(this->device.get(), &createInfo, nullptr, &layout) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create descriptor set");
        }
    }

    VKDescriptorSetLayout::VKDescriptorSetLayout(VKDescriptorSetLayout&& other) noexcept = default;

    VKDescriptorSetLayout& VKDescriptorSetLayout::operator=(VKDescriptorSetLayout&& other) noexcept = default;

    VKDescriptorSetLayout::~VKDescriptorSetLayout() {
        vkDestroyDescriptorSetLayout(device.get(), layout, nullptr);
    }
}