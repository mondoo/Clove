#include "Clove/Graphics/Vulkan/VKDescriptorSetLayout.hpp"

namespace clv::gfx::vk {
    static VkDescriptorType getDescriptorType(DescriptorType garlicType) {
        switch(garlicType) {
            case DescriptorType::UniformBuffer:
                return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            default:
                CLV_ASSERT(false, "{0}: Unkown type", CLV_FUNCTION_NAME);
                return VK_DESCRIPTOR_TYPE_MAX_ENUM;
        }
    }

    static VkShaderStageFlags getShaderStages(DescriptorStage garlicStage) {
        switch(garlicStage) {
            case DescriptorStage::Vertex:
                return VK_SHADER_STAGE_VERTEX_BIT;
            default:
                CLV_ASSERT(false, "{0}: Unkown type", CLV_FUNCTION_NAME);
                return 0;
        }
    }

    VKDescriptorSetLayout::VKDescriptorSetLayout(VkDevice device, DescriptorSetLayoutDescriptor descriptor) 
        : device(device){
        std::vector<VkDescriptorSetLayoutBinding> layoutBindings(std::size(descriptor.bindings));

        for(size_t i = 0; i < std::size(layoutBindings); ++i) {
            const DescriptorBindingInfo& bindingDescriptor = descriptor.bindings[i];

            layoutBindings[i].binding            = bindingDescriptor.binding;
            layoutBindings[i].descriptorType     = getDescriptorType(bindingDescriptor.type);
            layoutBindings[i].descriptorCount    = bindingDescriptor.arraySize;
            layoutBindings[i].stageFlags         = getShaderStages(bindingDescriptor.stage);
            layoutBindings[i].pImmutableSamplers = nullptr;
        }

        VkDescriptorSetLayoutCreateInfo createInfo{};
        createInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        createInfo.pNext        = nullptr;
        createInfo.flags        = 0;
        createInfo.bindingCount = std::size(layoutBindings);
        createInfo.pBindings    = std::data(layoutBindings);

        if(vkCreateDescriptorSetLayout(device, &createInfo, nullptr, &layout) != VK_SUCCESS) {
            GARLIC_LOG(garlicLogContext, Log::Level::Error, "Failed to create descriptor set");
        } 
    }

    VKDescriptorSetLayout::~VKDescriptorSetLayout() {
        vkDestroyDescriptorSetLayout(device, layout, nullptr);
    }

    VkDescriptorSetLayout VKDescriptorSetLayout::getLayout() const {
        return layout;
    }
}